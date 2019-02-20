//******************************************//
//**Program's type: AI**********************//
//**Programmer: Raal************************//
//**Language: C++ VS************************//
//**Created: 02.02.2019*********************//
//**Last update: 20.02.2019**************** //
//******************************************//
//******************************************//

#include <iostream>
#include <fstream>
#include <string.h>
#include <Conio.h>
#include "Network.h"
#include <random>

using namespace std;

const char filePathInput_Hidden[] = "IH.txt";   //название файла с набором весов (inp-hid layer)
const char filePathHidden_Output[] = "HO.txt";  //название файла с набором весов (hid-out layer)

network *newNet;                 //Ссылка на класс нейросети 
int createNetwork(int,int,int);  //функция создает файлы с случайными весами
int deleteNetwork();             //Функция очищает файлы весов
int init(int,int,int);           //Функция инициализирует начальные веса нейросети
int rewrite(int, int, int);      //Функция перезаписывает новые веса в файлы

int main()                       //Начало программы
{
	setlocale(LC_ALL, "rus");
	system("color F0");

	//***Переменные***//

	static bool isWork = true;   //вкл\выкл программу
	int inputLayerSize = 3;      //Размер входного слоя
	int hiddenLayerSize = 2;     //Размер скрытого слоя
	int outputLayerSize = 3;     //Размер выходного слоя
	int getKey;                  //Выбор пользователя (меню)
	double *actInput;            //Входной слой
	double *actOutput;           //Ожидаемый выходной слой 
	int realEpoch = 0;           //Значение эпохи храним в файле         

	//***Программа***//

	cout << "\t\t\tВыберите действие\n";
	while (isWork) 
	{
		cout << "0. create Network\n";
		cout << "1. get vector\n";
		cout << "2. teach Network\n";
		cout << "3. delete Network\n";
		cout << "4. Exit\n";
		
		getKey = _getch();          //Ожидание выбора пользователя
		switch (getKey) 
		{
		case 48:         //По нажатию 0
			createNetwork(inputLayerSize, hiddenLayerSize, outputLayerSize);
			break;       
			             
		case 49:         //По нажатию 1
			newNet = new network(inputLayerSize, hiddenLayerSize, outputLayerSize, 0); //Создаем объект нейросети
			init(inputLayerSize, hiddenLayerSize, outputLayerSize);                    //Инициализируем нейросеть
			actInput = new double[inputLayerSize];                                     //Выделяем память входного слоя
			cout << "Enter input vector with " << inputLayerSize << " values: ";       
			for (int i = 0; i < inputLayerSize; i++)                                   //Ввод входного слоя
			{
				cin >> actInput[i];
			}
			newNet->getActualOutput(actInput);     //Получаем выходной слой 
			delete newNet;                         //Очищаем память
			break;
		case 50:         //По нажатию 2
			newNet = new network(inputLayerSize, hiddenLayerSize, outputLayerSize, 0.4f, realEpoch); //Создаем объект нейросети
			init(inputLayerSize, hiddenLayerSize, outputLayerSize);                          //Инициализируем нейросеть

			actInput = new double[inputLayerSize];                                           //Выделяем память входного слоя
			actOutput = new double [outputLayerSize];                                        //Выделяем память выходного слоя

			cout << "Enter input vector with " << inputLayerSize << " values: ";
			for (int i = 0; i < inputLayerSize; i++)                                   //Ввод входного слоя
			{
				cin >> actInput[i];
			}

			cout << "Enter output vector with " <<outputLayerSize << " values: ";
			for (int i = 0; i < outputLayerSize; i++)                                  //Ввод выходного слоя
			{
				cin >> actOutput[i];
			}
			//actInput = new double[inputLayerSize] {1, 0, 1};   //пример
			//actOutput = new double[outputLayerSize] {0, 1, 1};


				if (newNet->train(actInput, actOutput) == 100)                  //Тренировка нейросети
				{                                                               //Если успешно
					realEpoch = newNet->epoch;
					rewrite(inputLayerSize, hiddenLayerSize, outputLayerSize);  //То перезаписываем файлы весов
					cout << "trainig completed! \n";
				}
				else
				{
					cout << "training was failed" << endl;
				}
			
			delete newNet;     //Очищаем память
			break;

		case 51:              //По нажатию 3
			deleteNetwork();  //Удаляем нейросеть
			break;
		case 52:              //По нажатию 4
			isWork = false;   //Завершаем программу
			break;
		default:
			break;
		}
		cout <<"_____________"<< endl << endl;
	}
	system("pause");
	return 0;               //Выход из программы
}

//i-размер входного слоя
//h-размер скрытого слоя
//o-размер выходного слоя
int createNetwork(int i, int h, int o)   
{
	//***Переменные***//

	int _size1 = i * h;    //Кол-во весов в 1 файле
	int _size2 = h * o;    //Кол-во весов во 2 файле
	double randVal;        //Переменная со случ. значением
	std::default_random_engine ranEngine;
	std::uniform_real_distribution<double> dist(-0.3, 0.3);
	ofstream fout1;       //Поток записи в файл 1
	ofstream fout2;       //Поток записи в файл 2

	//***Программа***//

	fout1.open(filePathInput_Hidden);  //Открываем файл 1
	fout2.open(filePathHidden_Output); //Открываем файл 2

	    //Файловая проверка
		if (!fout1) 
		{
			cout << "file does not exist: " << filePathInput_Hidden << endl;
			return 11;
		}
		if (!fout2)
		{
			cout << "file does not exist: " << filePathHidden_Output << endl;
			return 12;
		}

		for (int j = 0; j < _size1; j++)  //Записываем в файл 1
		{
			randVal = dist(ranEngine);    //Получаем случ. значение
			if(j == _size1 - 1)
			fout1 << randVal;
			else 
			fout1 << randVal <<" ";
		}
		for (int j = 0; j < _size2; j++)  //Записываем в файл 1
		{
			randVal = dist(ranEngine);    //Получаем случ. значение
			if (j == _size2 - 1)
			fout2 << randVal;
			else
			fout2 << randVal << " ";
		}
		fout1.clear();
		fout2.clear();
		fout2.close();
		fout2.close();
		cout << "\nNetwork was created!\n";
		return 10;
		
}

int deleteNetwork()    //
{
	ofstream fout;     //Открываем файлы для перезаписи
	fout.open(filePathInput_Hidden);
	fout.close();
	fout.open(filePathHidden_Output);
	fout.close();
	fout.clear();
	cout << "\nNetwork was removed!\n";
	return 30;
}


//inp-размер входного слоя
//hid-размер скрытого слоя
//otp-размер выходного слоя
int init(int inp, int hid, int otp)
{

	//***Переменные***//
	ifstream reader1;
	ifstream reader2;

	//***Программа***//

	reader1.open(filePathInput_Hidden);
	reader2.open(filePathHidden_Output);

	//Файловые проверки
	if (!reader1)
	{
		cout << "file does not exist: " << filePathInput_Hidden << endl;
		return 41;
	}
	if (!reader2)
	{
		cout << "file does not exist: " << filePathHidden_Output << endl;
		return 42;
	}
	if (reader1.eof() || reader2.eof())  //Пустые файлы
	{
		cout << "files are empty: " << filePathInput_Hidden << endl;
		return 43;
	}

	//Создаем динамическую матрицу весов 1
	newNet->ihConnect = new double *[hid];
	for (int i = 0; i < hid; i++) 
	{
		newNet->ihConnect[i] = new double[inp];
	}

	//Создаем динамическую матрицу весов 2
	newNet->hoConnect = new double *[otp];
	for (int i = 0; i < otp; i++) 
	{
		newNet->hoConnect[i] = new double[hid];
	}


	//Считываем с файла 1 значения и записываем в матрицу весов 1
	cout << "\n\tInput - hidden layer weights: \n";
		for (int i = 0; i < hid; i++) 
		{
			for (int j = 0; j < inp; j++) 
			{
				double val;
				reader1 >> val;
				newNet->ihConnect[i][j] = val;
				cout << newNet->ihConnect[i][j] << " ";
			}
			cout << endl;
		}

	//Считываем с файла 1 значения и записываем в матрицу весов 1
	cout << "\n\tHidden - output layer weights: \n";
		for (int i = 0; i < otp; i++)
		{
			for (int j = 0; j < hid; j++)
			{
				double val;
				reader2 >> val;
				newNet->hoConnect[i][j] = val;
				cout << newNet->hoConnect[i][j] << " ";
			}
			cout << endl;
		}


	cout << endl << endl;
	reader1.close();
	reader2.close();
	cout << "\nNetwork was initialized!\n";
	return 40;
}


//inp-размер входного слоя
//hid-размер скрытого слоя
//otp-размер выходного слоя
int rewrite(int inp, int hid, int otp)
{

    //***Переменные***//

	double val;         //Хранение текущего значения
	ofstream rewrite_1; //Поток записи в файл 1	
	ofstream rewrite_2; //Поток записи в файл 2	

	//***Программа***//

	cout << "\n\n\tNew weights: \n";
	cout << "Input-hidden: \n";
	rewrite_2.open(filePathHidden_Output);
	rewrite_1.open(filePathInput_Hidden);

	//Считываем с матриц весов 1 веса и записываем в файл 1
	for (int i = 0; i < hid; i++)
	{
		for (int j = 0; j < inp; j++)
		{
			val = newNet->ihConnect[i][j];
			if ((i == hid - 1) && (j == inp - 1))
				rewrite_1 << val;
			else
			rewrite_1 << val << " ";
		}
	}

	cout << "\nHidden - output: \n";

	//Считываем с матриц весов 1 веса и записываем в файл 1
	for (int i = 0; i < otp; i++)
	{
		for (int j = 0; j < hid; j++)
		{
			val = newNet->hoConnect[i][j] ;
			if ((i == otp - 1) && (j == hid - 1))
			    rewrite_2 << val;
			else
				rewrite_2 << val << " ";
		}
	}
	rewrite_1.close();
	rewrite_2.close();
	return 50;
}