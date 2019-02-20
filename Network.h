#pragma once
class network
{
public:

	double *hiddenLayer;    //Посчитанный скрытый слой нейросети
	double *outputActual;   //Посчитанный выходной слой нейросети

	double *hiddenLayer_Activated;  //Активированный скрытый  слой
	double *outputActual_Activated; //Активированный выходной  слой

	double **ihConnect;    //Матрица весов слоев input-hidden
	double **hoConnect;    //Матрица весов слоев hidden-output

	int train(double[], double[]);    //Функция тренировки нейросети backpropagation

	void showEpochNRate();               //Вывод эпохи и рейта
	int getActualOutput(double input[]); //Получение выходного слоя

	network(int,int,int, double, int);  //Конструктор для тренировки
	network(int, int, int, int);        //Конструктор для подсчета
	~network();                         //Деструктор по умолчанию

	int epoch;           //Эпоха

private:
	int iSize; //Размер входного слоя
	int hSize; //Размер скрытого слоя
	int oSize; //Размер выходного слоя
    int rate;            //Рейт
	double learningRate; //Скорость обучения [0..1]

	double activization_sigmoid(double);  //Активация сигмоидом
};

/*  I    H    O
    0         0
	0  - 0 -  0
	0  - 0 -  0
	0         0
	  ih  ho
  iSize hSize oSize
*/
