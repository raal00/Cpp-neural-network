#include "Network.h"
#include <fstream>
#include <iostream>

#define MAX_RATE 20 

network::network(int iSize, int hSize, int oSize, double lr, int epoch)  //Конструктор 1
{
	this->iSize = iSize;
	this->hSize = hSize;
	this->oSize = oSize;
	this->learningRate = lr;
	rate = 0;
	this->epoch = epoch;
}
network::network(int iSize, int hSize, int oSize, int epoch)            //Конструктор 2
{
	this->iSize = iSize;
	this->hSize = hSize;
	this->oSize = oSize;
	rate = 0;
	this->epoch = epoch;
}

network::~network() //Деструктор по умолчанию
{

}

//***activization functions***//
double network::activization_sigmoid(double val) 
{
	return 1. / (1 + exp(-val));
}

//***functions***//
void network::showEpochNRate()
{
	std::cout << "\nRate: " << rate << "\tEpoch: " << epoch << std::endl;
}


int network::getActualOutput(double input[])
{	
	hiddenLayer = new double[hSize];
	outputActual = new double[oSize];

	hiddenLayer_Activated = new double[hSize];
	outputActual_Activated = new double[oSize];

	std::cout << "\n[ECHO] Input: ";
	for (int i = 0; i < iSize; i++) 
	{
		std::cout << input[i] << " ";
	}
	std::cout << std::endl << std::endl;


	for (int i = 0; i < hSize; i++) 
	{
		hiddenLayer[i] = 0;
	}

	for (int i = 0; i < oSize; i++) 
	{
		outputActual[i] = 0;
	}
	/*
	Найдем ошибки выходного и крытого слоев нейросети
	
	*/
	if (ihConnect != NULL && hoConnect != NULL)
	{
		for (int i = 0; i < hSize; i++)              //берем каждый нейрон из скрытого слоя
		{
			for (int j = 0; j < iSize; j++)
			{
				hiddenLayer[i] += ihConnect[i][j] * input[j];  //находим его значение по весам связей 	
			}
			hiddenLayer_Activated[i] = activization_sigmoid(hiddenLayer[i]); //Активируем нейроны
		} 
		for (int i = 0; i < oSize; i++)            //берем каждый нейрон из выходного слоя
		{
			for (int j = 0; j < hSize; j++)
			{
				outputActual[i] += hoConnect[i][j] * hiddenLayer_Activated[j]; //находим его значение по весам связей 
			}
			outputActual_Activated[i] = activization_sigmoid(outputActual[i]);  //активируем выходной нейрон
			std::cout << outputActual_Activated[i] << std::endl;
		}
		for (int i = 0; i < iSize; i++)              //Вывод готового результата 
		{
			outputActual_Activated[i] >= 0.5f ? std::cout << "1 " : std::cout << "0 ";
		}
		std::cout << std::endl;
		delete hiddenLayer;             //Освобождаем память
		delete outputActual;
		return 90;
	}
	else 
	{
		std::cout << "Error" << std::endl;
		return 91;
	}
}


//Функция тренировки нейросети backpropagation
/*
	Найдем ошибки выходного и крытого слоев нейросети
	1. Ошибки выходных = вектор ожидавемого рез. - полученный
	2. Ошибки скрытых нейронов:
	Т.к. каждый скрытый нейрон имеет связь с выходными, найдем
	общую ошибку нейрона с помощбю алг. суммы Wij*oi
	i = 1..sizeof output  j = 1..sizeof hidden
	W - вес связи скрытого нейрона с выходными
	o - полученное активированное значение выходного нейрона

	Зная ошибки нейронов, начнем сводить веса связей
	Wij += delta[i] * F'(S) * f[j] * lr
	l - слой
	delta[i] - ошибка нейрона к которому идет связь (l)
	f[j] - активированное значение исходящего нейрона (l-1)
	F'(S) - градиент, указывающий в сторону изм. веса
	lr - скорость обучения [0..1]
*/
int network::train(double input[], double output[]) 
{

	 //***Переменные***//

	 double _RMSE = 0;                        //Показатель сходимости
	 double *deltaOutput = new double[oSize]; //Дельта выходного
	 double *deltaHidden = new double[hSize]; //Дельта скрытого

	 //***Программа***//

	 if (getActualOutput(input) != 90)  //Получаем значения сети
	 {
		 std::cout << "network error!\n";
		 return 101;
	 }
	 for (int i = 0; i < oSize; i++)   //ищем дельту у каждого выходного нейрона 
	 { 
		deltaOutput[i] = output[i] - outputActual_Activated[i];
		_RMSE += deltaOutput[i] * deltaOutput[i];
     }
	 _RMSE /= oSize;
	 _RMSE = sqrt(_RMSE);
     std::cout << "RMSE: " << _RMSE << std::endl;   //На сколько сеть сошлась

	 //*********************************************//
	 for (int i = 0; i < hSize; i++)                //Ищем дельту выходного слоя
	 {
		 deltaHidden[i] = 0;
		 for (int j = 0; j < oSize; j++) 
		 {
			 deltaHidden[i] += hoConnect[j][i] * deltaOutput[j]; //
		 }
	 }

	 //Настройка весов
	 for (int i = 0; i < hSize; i++) 
	 {
		 for (int j = 0; j < iSize; j++) 
		 {
			 ihConnect[i][j] += deltaHidden[i] * hiddenLayer_Activated[i] * (1 - hiddenLayer_Activated[i]) * input[j] * learningRate;
		 }
	 }
	 for (int i = 0; i < iSize; i++) 
	 {
		 for (int j = 0; j < hSize; j++) 
		 {
			 hoConnect[i][j] += deltaOutput[i] * outputActual_Activated[i] * (1 - outputActual_Activated[i]) * hiddenLayer_Activated[j] * learningRate;
		 }
	 }
	rate++;
	if (rate == MAX_RATE) 
	{
		rate = 0;
		epoch++;
	}
	showEpochNRate();
	delete outputActual_Activated;
	delete hiddenLayer_Activated;
	return 100;
}
