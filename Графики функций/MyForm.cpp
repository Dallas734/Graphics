#include "MyForm.h"


using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Графикифункций::MyForm form;
	Application::Run(% form);

}

System::Void Графикифункций::MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
	double pW = pictureBox1->Width;
	double pH = pictureBox1->Height;

	float miny;
	float maxy;
	double uintX;
	double uintY;

	Bitmap^ img = gcnew Bitmap(pW, pH); // создаем поверхность для рисования (изображение) с размером элемента PictureBox.

	Graphics^ g = Graphics::FromImage(img); // создаем устройство для рисования на поверхности.
	g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias; // Качество.

	// Создаем кисти.
	Pen^ bp = gcnew Pen(Color::Black, 1);

	Pen^ bg1 = gcnew Pen(Color::Green, 2);
	Pen^ bg2 = gcnew Pen(Color::Green, 2);
	bg1->StartCap = System::Drawing::Drawing2D::LineCap::ArrowAnchor;
	bg2->EndCap = System::Drawing::Drawing2D::LineCap::ArrowAnchor;

	Pen^ br = gcnew Pen(Color::Red, 3);
	br->StartCap = System::Drawing::Drawing2D::LineCap::Round;
	br->EndCap = System::Drawing::Drawing2D::LineCap::Round;

	Brush^ brush = Brushes::Black;

	// Шрифт
	System::Drawing::Font^ font = gcnew System::Drawing::Font("Times New Roman", 14);

	// Если поля не заполнены, то используем значения по умолчанию. 
	if (textBoxA->Text == "" || textBoxB->Text == "")
	{
		MessageBox::Show("Используются параметры по умолчанию!\nA = -10\nB = 10\ndx = 0,5", "Внимание!");
		DefaultParams();
	}
	else
	{
		try
		{
			a = Convert::ToSingle(textBoxA->Text);
			b = Convert::ToSingle(textBoxB->Text);
			if (textBoxDelta->Text != "")
			{
				delta = Convert::ToSingle(textBoxDelta->Text);
			}
			else if (radioButton1->Checked == true)
			{
				delta = 0.1;
			}
			else if (radioButton2->Checked == true)
			{
				delta = 0.5;
			}
			else if (radioButton3->Checked == true)
			{
				delta = 1;
			}
			else
			{
				delta = 0.5;
			}
		}
		catch (System::Exception^)
		{
			MessageBox::Show("Неверно введены значения!", "Ошибка!");
			return;
		}
	}

	float x = a;
	float y;

	System::Collections::Generic::List<PointF>^ points = gcnew System::Collections::Generic::List<PointF>(); // Коллекция точек графика.
	
	// Не выбрали ни один из графиков.
	if (radioButtonSin->Checked == false && radioButtonCos->Checked == false && textBox1->Text == "")
	{
		MessageBox::Show("Вы не ввели или не выбрали график функции!", "Внимание!");
		return;
	}

	if ((abs(a) + abs(b)) < delta)
	{
		MessageBox::Show("Неверно введен шаг или границы!", "Внимание!");
		return;
	}
	else if (radioButtonSin->Checked == true && radioButtonCos->Checked == false && textBox1->Text == "")
	{
		int num = (abs(a) + abs(b)) / delta + 1;
		int i = 0;

		char* str1 = "y = sin(x)";
		label7->ForeColor = Color::Red;
		label7->Text = Convert_char_to_String(str1);

		// Ищем максимальное и минимальное значение.
		miny = sin(x);
		maxy = sin(x);
		while (x <= b)
		{
			y = sin(x);

			if (miny > y)
			{
				miny = y;
			}
			if (maxy < y)
			{
				maxy = y;
			}

			x += delta;
		}

		DrawArea(g, bp, bg1, bg2, br, font, brush, pW, pH, a, b, miny, maxy, uintX, uintY);

		x = a;
		while (x <= b)
		{
			// рисуем график
			y = sin(x);

			points->Add(PointF(x * (float)uintX, y * (float)uintY)); // Добавляем точки (сразу переводим в экранные единицы).

			x += delta;
		}

		g->DrawCurve(br, points->ToArray()); // Рисование точек графика.

		delete g; // Освобождение устройства для рисования.

		this->pictureBox1->Image = img; // Присвоение и отображение изображения в PictureBox.
	}
	else if (radioButtonSin->Checked == false && radioButtonCos->Checked == true && textBox1->Text == "")
	{
		int num = (abs(a) + abs(b)) / delta + 1;
		int i = 0;

		char* str1 = "y = cos(x)";
		label7->ForeColor = Color::Red;
		label7->Text = Convert_char_to_String(str1);
		// Ищем максимальное и минимальное значение.
		miny = cos(x);
		maxy = cos(x);
		while (x <= b)
		{
			y = cos(x);

			if (miny > y)
			{
				miny = y;
			}
			if (maxy < y)
			{
				maxy = y;
			}

			x += delta;
		}

		DrawArea(g, bp, bg1, bg2, br, font, brush, pW, pH, a, b, miny, maxy, uintX, uintY);

		x = a;
		while (x <= b)
		{
			// рисуем график
			y = cos(x);

			points->Add(PointF(x * (float)uintX, y * (float)uintY)); // Добавляем точки (сразу переводим в экранные единицы).

			x += delta;
		}
		g->DrawCurve(br, points->ToArray()); // Рисование точек графика.

		delete g; // Освобождение устройства для рисования.

		this->pictureBox1->Image = img; // Присвоение и отображение изображения в PictureBox.
	}
	else
	{
		try 
		{
			// Считываем выражение с textBox, преобразуем его в функцию и строим график.
			char* function = revPoland(Convert_String_to_char(textBox1->Text));

			// Добавляем название графика.
			char yStr[20] = "y = ";
			char* str1 = Convert_String_to_char(textBox1->Text);
			strcat(yStr, str1);
			label7->ForeColor = Color::Red;
			label7->Text = Convert_char_to_String(yStr);

			// Ищем максимальное и минимальное значение.
			miny = Result(function, x);
			maxy = Result(function, x);
			do
			{
				miny = Result(function, x);
				maxy = Result(function, x);
				x += delta;
			} while (miny == INFINITY);
			x = a;
			while (x <= b)
			{
				y = Result(function, x);

				if (miny > y && y != INFINITY)
				{
					miny = y;
				}
				if (maxy < y && y != INFINITY)
				{
					maxy = y;
				}

				x += delta;
			}

			DrawArea(g, bp, bg1, bg2, br, font, brush, pW, pH, a, b, miny, maxy, uintX, uintY);
			// Добавляем точки
			x = a;
			while (x <= b)
			{
				y = Result(function, x);
				
				if (y == INFINITY)
				{
					points->Add(PointF(INFINITY, INFINITY));
				}
				else
				{
					points->Add(PointF(x * (float)uintX, y * (float)uintY));
				}
				
				x += delta;
			}

			System::Collections::Generic::List<PointF>^ tempPoints = gcnew System::Collections::Generic::List<PointF>(); // Коллекция точек графика.
			start = 0;
			end = 0;
			int count = 0;
			for (; start < points->Count; start++)
			{
				count++;
				if (points[start].X == INFINITY && points[start].Y == INFINITY && start != 0)
				{
					// Берем диапазон до асимптоты.
					tempPoints = points->GetRange(end, count - 1);
					end = start + 1;
					count = 0;
					g->DrawCurve(br, tempPoints->ToArray()); // Рисование точек графика.
				}
				else if (start == points->Count - 1)
				{
					tempPoints = points->GetRange(end, count);
					end = start + 1;
					count = 0;
					g->DrawCurve(br, tempPoints->ToArray()); // Рисование точек графика.
				}
			}

			delete g; // Освобождение устройства для рисования.
			this->pictureBox1->Image = img; // Присвоение и отображение изображения в PictureBox.
		}
		catch (System::Exception^)
		{
			MessageBox::Show("Неверно введены значения!", "Ошибка!");
		}
	}
}

System::Void Графикифункций::MyForm::выходToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	Application::Exit();
}

System::Void Графикифункций::MyForm::оПрограммеToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	MessageBox::Show("Программа строит графики введенной функции\nВерсия: 6.0\n©Вернидуб Марк 1-41, 2022", "Информация об программе");
}

System::Void Графикифункций::MyForm::справкаToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	MessageBox::Show("Знаки операций: \n\"+\" - сложение\n\"-\" - вычитание\n\"*\" - умножение\n\"/\" - деление\n\"^\" - возведение в степень\n\nФункции:\nsin() - синус\ncos() - косинус\nln() - натуральный логарифм\nlg() - десятичный логарифм\nДесятичные числа записываются через запятую!\ne - экспонента\n pi - число Пи", "Справка");
}

void Графикифункций::MyForm::DefaultParams()
{
	a = -10;
	b = 10;
	delta = 0.5;
}

// конвертируем System::string ^ в char*
char* Графикифункций::MyForm::Convert_String_to_char(System::String^ string)
{
	using namespace System::Runtime::InteropServices;
	return (char*)(void*)Marshal::StringToHGlobalAnsi(string);
}

System::String^ Графикифункций::MyForm::Convert_char_to_String(char* ch)
{
	char* chr = new char();
	chr = ch;
	System::String^ str;
	for (int i = 0; chr[i] != '\0'; i++)
	str += wchar_t(ch[i]);//wchar_t() - преобразует char в String
	return str;
}

int Графикифункций::MyForm::Prior(char a)
{
	switch (a)
	{
	case 'u': return 5;
	case '^': return 4;
	case '*':
	case '/': return 3;
	case '-':
	case '+': return 2;
	case '(': return 1;
	}
	return 0;
}

Stack1* Графикифункций::MyForm::InputStack(Stack1* t, char s)
{
	Stack1* t1 = (Stack1*)malloc(sizeof(Stack1));
	t1->c = s;
	t1->Prev = t;
	return t1;
}

Stack1* Графикифункций::MyForm::OutputStack(Stack1* t, char* s)
{
	// Stack1* t1 = t;
	*s = t->c;
	t = t->Prev;
	// free(t1);
	return t;
}

float Графикифункций::MyForm::Result(char* buf, float x)
{
	double stack[256]{ 0 };
	int i = -1;
	int n = 0;
	int f = 0;
	int sp = 0;
	char temp[10]{ 0 };

	while (buf[f] != '\0') {

		while (buf[f + n] != ' ')
		{
			n++;
		}

		strncpy_s(temp, buf + f, n);
		f += n;

		if (strcmp(temp, "+") == 0)
		{
			stack[sp - 2] = stack[sp - 2] + stack[sp - 1];
			sp--;
		}
		else if (strcmp(temp, "-") == 0)
		{
			stack[sp - 2] = stack[sp - 2] - stack[sp - 1];
			sp--;
		}
		else if (strcmp(temp, "*") == 0)
		{
			stack[sp - 2] = stack[sp - 2] * stack[sp - 1];
			sp--;
		}
		else if (strcmp(temp, "/") == 0)
		{
			stack[sp - 2] = stack[sp - 2] / stack[sp - 1];
			sp--;
		}
		else if (strcmp(temp, "^") == 0)
		{
			stack[sp - 2] = pow(stack[sp - 2], stack[sp - 1]);
			sp--;
		}
		else if (strcmp(temp, "sin") == 0)
		{
			stack[sp - 1] = sin(stack[sp - 1]);
		}
		else if (strcmp(temp, "cos") == 0)
		{
			stack[sp - 1] = cos(stack[sp - 1]);
		}
		else if (strcmp(temp, "ln") == 0) // Натуральный логарифм
		{
			// Поставить условие логарифмам.
			stack[sp - 1] = log(stack[sp - 1]);
		}
		else if (strcmp(temp, "lg") == 0) // Десятичный логарифм
		{
			stack[sp - 1] = log10(stack[sp - 1]);
		}
		else if (strcmp(temp, "tg") == 0)
		{
			stack[sp - 1] = tan(stack[sp - 1]);
		}
		else if (strcmp(temp, "sqrt") == 0)
		{
			stack[sp - 1] = sqrt(stack[sp - 1]);
		}
		else if (strcmp(temp, "u") == 0) // Унарный минус
		{
			stack[sp - 1] = stack[sp - 1] * (-1);
		}
		else if (strcmp(temp, "x") == 0)
		{
			stack[sp++] = x;
		}
		else if (strcmp(temp, "e") == 0)
		{
			stack[sp++] = EXP;
		}
		else if (strcmp(temp, "pi") == 0)
		{
			stack[sp++] = PI;
		}
		else if (strcmp(temp, "") != 0)
		{
			stack[sp++] = atof(temp);
		}

		memset(temp, NULL, sizeof(char));
		f++;
		n = 0;
	}

	return stack[sp - 1];
}

char* Графикифункций::MyForm::revPoland(char* input)
{
	Stack1* op = nullptr;// Стек операций пуст
	char a;
	char* output = new char[50];// Выходная строка
	char buf[10]{ 0 };
	int k = 0, i = 0, m = 0, f = 0, n; // Индексы для строк

	try
	{
		while (input[k] != '\0') // Анализируем символы входной строки.
		{
			n = 0;
			// Если символ ")" выталкиваем из стека в выходную строку все операции.
			if (input[k] == ')')
			{
				// До открывающейся скобки.
				while (op->c != '(')
				{
					op = OutputStack(op, &a); // Считываем элемент из стека.
					if (!op)
					{
						a = '\0';
					}

					functions(i, output, a);
				}
				op = op->Prev;
			}
			// Если символ строки - буква, то заносим ее в выходную строку.
			if (input[k] >= 'a' && input[k] <= 'z')
			{
				f = k;
				while (input[f] >= 'a' && input[f] <= 'z')
				{
					n++;
					f++;
				}
				strncpy_s(buf, input + k, n);
				if (strcmp(buf, "sin") == 0)
				{
					op = InputStack(op, 's');
					k = f - 1;
				}
				else if (strcmp(buf, "cos") == 0)
				{
					op = InputStack(op, 'c');
					k = f - 1;
				}
				else if (strcmp(buf, "ln") == 0)
				{
					op = InputStack(op, 'n');
					k = f - 1;
				}
				else if (strcmp(buf, "lg") == 0)
				{
					op = InputStack(op, 'g');
					k = f - 1;
				}
				else if (strcmp(buf, "tg") == 0)
				{
					op = InputStack(op, 't');
					k = f - 1;
				}
				else if (strcmp(buf, "sqrt") == 0)
				{
					op = InputStack(op, 'q');
					k = f - 1;
				}
				else if (strcmp(buf, "pi") == 0)
				{
					output[i++] = 'p';
					output[i++] = 'i';
					output[i++] = ' ';
					k = f;
				}
				else if (n == 1)
				{
					output[i++] = input[k];
					output[i++] = ' ';
					k = f - 1;
				}
			}

			// Если символ цифра, то заносим ее в выходную строку
			if (input[k] > -1 && input[k] < 255)
			{
				if (isdigit(input[k]))
				{
					m = k;
					while (isdigit(input[m]) || input[m] == '.')
					{
						output[i++] = input[m];
						m++;
					}
					output[i++] = ' ';
					k = m - 1;
				}
			}
			
			// Если символ - открывающаяся скобка, то записываем ее в стек.
			if (input[k] == '(')
			{
				op = InputStack(op, input[k]);
			}

			// Если символ - знак операции, переписываем из стека в строку output все операции с большим или равным приоритетом.
			if (input[k] == '+' || input[k] == '-' || input[k] == '*' || input[k] == '/' || input[k] == '^')
			{
				// Проверка на унарный минус.
				// Если последний элемент открывающаяся скобка или он отсутсвует, а сейчас -, значит он унарный.
				if (input[k] == '-' && (op == nullptr || op->c == '(') && (output[i - 2] == NULL || i < 2))
				{
					input[k] = 'u';
				}

				// пока на вершине стека префиксная функция…
				//	ИЛИ операция на вершине стека приоритетнее op->c;
				// выталкиваем верхний элемент стека в выходную строку.
				//	помещаем операцию op в стек.
				while (op != NULL && (Prior(op->c) >= Prior(input[k]) || op->c == 's' || op->c == 'c' || op->c == 'n' || op->c == 'g' || op->c == 't' || op->c == 'q'))
				{
					op = OutputStack(op, &a); // Извлекаем из стека символ output[i++] = a.
					functions(i, output, a);
					output[i++] = ' ';
				}
				op = InputStack(op, input[k]); // Текущий символ в стек.

			}
			k++;
		} // Конец цикла анализа входной строки.

		// Если стек не пуст, переписываем все операции в выходную строку.
		while (op != NULL)
		{
			op = OutputStack(op, &a);
			functions(i, output, a);
		}

		output[i] = '\0'; // Добавляем в конец строки терминирующий ноль.
	}
	catch (System::Exception^)
	{
		MessageBox::Show("Ошибка!", "Внимание!");
	}
	return output;
}

void Графикифункций::MyForm::functions(int& i, char output[], char a)
{
	if (a == 's')
	{
		output[i++] = 's';
		output[i++] = 'i';
		output[i++] = 'n';
		output[i++] = ' ';
	}
	else if (a == 'c')
	{
		output[i++] = 'c';
		output[i++] = 'o';
		output[i++] = 's';
		output[i++] = ' ';
	}
	else if (a == 'n')
	{
		output[i++] = 'l';
		output[i++] = 'n';
		output[i++] = ' ';
	}
	else if (a == 'g')
	{
		output[i++] = 'l';
		output[i++] = 'g';
		output[i++] = ' ';
	}
	else if (a == 't')
	{
		output[i++] = 't';
		output[i++] = 'g';
		output[i++] = ' ';
	}
	else if (a == 'q')
	{
		output[i++] = 's';
		output[i++] = 'q';
		output[i++] = 'r';
		output[i++] = 't';
		output[i++] = ' ';
	}
	else
	{
		output[i++] = a;
		output[i++] = ' ';
	}
}

// Показать/Скрыть название функции.
System::Void Графикифункций::MyForm::button2_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (label7->Visible)
	{
		label7->Visible = false;
	}
	else
	{
		label7->Visible = true;
	}
}

void Графикифункций::MyForm::DrawArea(Graphics^ g, Pen^ bp, Pen^ bg1, Pen^ bg2, Pen^ br, System::Drawing::Font^ font, Brush^ brush, double pW, double pH, float a, float b, float miny, float maxy, double& uintX, double& uintY)
{
	int countX;
	int countY;

	int axisY;
	int axisX;

	float xTT;
	float yTT;

	bool Xexists = false;
	bool Yexists = false;
	bool ScaleYExists = false;

	// Округление.
	int minX = round(a);
	int maxX = round(b);
	int minY = round(miny);
	int maxY = round(maxy);
	int yy = maxY;
	int scaleX = 1;
	int scaleY = 1;

	// Считаем коэфициенты.
	if (maxY > 0 && minY > 0)
	{
		uintY = ceil(pH / (maxY - minY));
	}
	else if (maxY < 0 && minY < 0)
	{
		uintY = ceil(pH / (abs(minY) - abs(maxY)));
	}
	else
	{
		uintY = ceil(pH / (abs(minY) + abs(maxY)));
	}
	
	if (minX > 0 && maxX > 0)
	{
		uintX = ceil(pW / (maxX - minX));
	}
	else if (minX < 0 && maxX < 0)
	{
		uintX = ceil(pW / (abs(minX) - abs(maxX)));
	}
	else
	{
		uintX = ceil(pW / (abs(minX) + abs(maxX)));
	}	

	// Ищем коэффициент масштабирования для делений
	// OY
	if (uintY <= 15)
	{
		for (int i = 120; i >= 2; i--)
		{
			if (maxY % i == 0)
			{
				scaleY = i;
				ScaleYExists = true;
				break;
			}	
		}
	}
	if (uintX <= 15)
	{
		for (int i = 9; i >= 2; i--)
		{

			if (maxX % i == 0)
			{
				scaleX = i;
				break;
			}
		}
	}

	// Поиск пересечения осей.
	// Существует ось Ox.
	int minYY = minY;
	int maxYY = maxY;
	if (minYY <= 0 && maxYY >= 0)
	{
		do
		{
			countY++;
			axisX = --maxYY;
		} while (axisX != 0);
		Xexists = true;
	}
	// Рисуем линии по Ox
	for (int i = 0; i <= pW; i += uintX * scaleX)
	{
		g->DrawLine(bp, i, 0, i, pH);
	}

	// Существует ось Oy.
	int minXX = abs(minX);
	int maxXX = maxX;
	if (minX <= 0 && maxX >= 0)
	{
		do
		{
			countX++;
			axisY = --minXX;
		} while (axisY != 0);
		Yexists = true;
		g->DrawLine(bg1, countX * uintX, 0, countX * uintX, pH);
	}

	// Рисуем линии по Oy
	// 
	// Коэффициент масштабирования графика в общем.
	if (maxY > 0 && minY > 0)
	{
		g->ScaleTransform(1, round(pH / (((maxY - minY) * uintY)) * 1000) / 1000 - 0.01);
	}
	else if (maxY < 0 && minY < 0)
	{
		g->ScaleTransform(1, round(pH / (((abs(minY) - abs(maxY)) * uintY)) * 1000) / 1000 - 0.01);
	}
	else
	{
		g->ScaleTransform(1, round(pH / (((abs(minY) + abs(maxY)) * uintY)) * 1000) / 1000 - 0.01);
	}

	int parY = (countY + abs(minY)) * uintY;
	for (int i = 0; i <= parY;)
	{
		if (i != countY * uintY || Xexists == false)
		{
			g->DrawLine(bp, 0, i, pW, i);
		}
		else if (Xexists == true)
		{
			// Рисуем ось Ox
			g->DrawLine(bg2, 0, countY * uintY, pW, countY * uintY);
		}

		if (Xexists == false && i == 0)
		{
			if (maxY < 0 && minY < 0)
			{
				parY = (abs(minY) - abs(maxY)) * uintY;
			}
			else
			{
				parY = (abs(maxY) - abs(minY)) * uintY;
			}
		}

		if (ScaleYExists)
		{
			i += scaleY;
		}
		else
		{
			i += uintY * scaleY;
		}
	}
	g->ScaleTransform(1, 1);


	// Рисуем деления.
	// Ox
	float numX = a;
	for (int i = 0; i <= pW; i += uintX * scaleX)
	{
		if (Xexists)
		{
			g->DrawLine(br, i, countY * uintY - 5, i, countY * uintY + 5);
		}

		if (minY > 0 && maxY > 0)
		{
			if (numX == a)
			{
				g->DrawString(System::Convert::ToString(numX), font, brush, i + 5, pH - 2);
			}
			else
			{
				g->DrawString(System::Convert::ToString(numX), font, brush, i - 10, pH - 20);
			}
		}
		else if (minY < 0 && maxY < 0)
		{
			if (numX == a)
			{
				g->DrawString(System::Convert::ToString(numX), font, brush, i - 3, 10);
			}
			else
			{
				g->DrawString(System::Convert::ToString(numX), font, brush, i - 25, 10);
			}
		}
		else
		{
			if (Xexists)
			{
				if (countY == 0)
				{
					g->DrawString(System::Convert::ToString(numX), font, brush, i - 10, countY * uintY + 23);
				}
				else
				{
					g->DrawString(System::Convert::ToString(numX), font, brush, i - 10, countY * uintY - 23);

				}
			}
		}
		numX += scaleX;
	}

	// Oy
	double c = 1;
	if (yy == 0)
	{
		c = (double)scaleY / (minY * uintY);
	}
	else
	{
		c = (double)scaleY / (yy * uintY);
	}

	float numY = yy;
	float del;
	if (numY == 0 && ScaleYExists && uintY != 1)
	{
		del = round((abs(minY) * c) * 10) / 10;
	}
	else if (ScaleYExists == false || uintY == 1)
	{
		del = scaleY;
	}
	else
	{
		del = round((numY * c) * 10) / 10;
	}

	for (int i = 0; i <= 2 * parY;)
	{
		// Деления
		if (Yexists)
		{
			g->DrawLine(br, countX * uintX - 5, i, countX * uintX, i);
		}

		if (numY == yy)
		{
			if (a < 0 && b <= 0 && i != countY * uintY)
			{
				g->DrawString(System::Convert::ToString(round(numY * 10) / 10), font, brush, pW - 30, i - 3);
			}
			else if (a >= 0 && b > 0 && i != countY * uintY)
			{
				g->DrawString(System::Convert::ToString(round(numY * 10) / 10), font, brush,  15, i - 3);
			}
			else if (i != countY * uintY)
			{
				g->DrawString(System::Convert::ToString(round(numY * 10) / 10), font, brush, countX * uintX + 15, i + 3);
			}
		}
		else if (numY == minY)
		{
			if (a < 0 && b <= 0 && i != countY * uintY)
			{
				g->DrawString(System::Convert::ToString(round(numY * 10) / 10), font, brush, pW - 30, i - 20);
			}
			else if (a >= 0 && b > 0 && i != countY * uintY)
			{
				g->DrawString(System::Convert::ToString(round(numY * 10) / 10), font, brush, 15, i - 20);
			}
			else if (i != countY * uintY)
			{
				g->DrawString(System::Convert::ToString(round(numY * 10) / 10), font, brush, countX* uintX + 15, i - 20);
			}
		}
		else 
		{
			if (a < 0 && b <= 0 && i != countY * uintY)
			{
				g->DrawString(System::Convert::ToString(round(numY * 10) / 10), font, brush, pW - 30, i - 10);
			}
			else if (a >= 0 && b > 0 && i != countY * uintY)
			{
				g->DrawString(System::Convert::ToString(round(numY * 10) / 10), font, brush, 15, i - 10);
			}
			else if (i != countY * uintY)
			{
				g->DrawString(System::Convert::ToString(round(numY * 10) / 10), font, brush, countX* uintX + 15, i - 10);
			}
		}

		if (Xexists == false && i == 0)
		{
			if (maxY < 0 && minY < 0)
			{
				parY = (abs(minY) - abs(maxY)) * uintY;
			}
			else
			{
				parY = (abs(maxY) - abs(minY)) * uintY;
			}
		}

		if (ScaleYExists)
		{
			i += scaleY;
		}
		else
		{
			i += uintY * scaleY;
		}
		
		// numY -= scaleY;
		if (del > 0)
		{
			numY -= del;
		}
		else
		{
			numY -= del * (-1);
		}
	}
	
	if (Xexists && b != 0)
	{
		g->DrawString("x", font, brush, float(pW) - 15, countY * uintY - 20);
	}
	if (Yexists && maxY != 0)
	{
		g->DrawString("y", font, brush, countX * uintX + 5, 0);
	}

	// Поиск координат точки пересечения осей.
	if (minX < 0 && maxX < 0)
	{
		xTT = abs(minX) * uintX; 
	}
	else if (minX > 0 && maxX > 0)
	{
		xTT = -minX * uintX;
	}
	else if (minX < 0 && maxX > 0)
	{
		xTT = abs(minX) * uintX;
	}
	else if (minX == 0)
	{
		xTT = 0;
	}
	else if (maxX == 0)
	{
		xTT = abs(minX) * uintX;
	}

	if (minY < 0 && maxY < 0)
	{
		yTT = -abs(maxY) * uintY;
	}
	else if (minY > 0 && maxY > 0)
	{
		yTT = maxY * uintY; 
	}
	else if (minY < 0 && maxY > 0)
	{
		yTT = maxY * uintY;
	}
	else if (minY == 0)
	{
		yTT = abs(maxY) * uintY;
	}
	else if (maxY == 0)
	{
		yTT = 0;
	}

	g->ScaleTransform(1, -1);
	g->TranslateTransform(xTT, -yTT);
}
	
