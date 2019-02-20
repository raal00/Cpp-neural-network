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

const char filePathInput_Hidden[] = "IH.txt";   //�������� ����� � ������� ����� (inp-hid layer)
const char filePathHidden_Output[] = "HO.txt";  //�������� ����� � ������� ����� (hid-out layer)

network *newNet;                 //������ �� ����� ��������� 
int createNetwork(int,int,int);  //������� ������� ����� � ���������� ������
int deleteNetwork();             //������� ������� ����� �����
int init(int,int,int);           //������� �������������� ��������� ���� ���������
int rewrite(int, int, int);      //������� �������������� ����� ���� � �����

int main()                       //������ ���������
{
	setlocale(LC_ALL, "rus");
	system("color F0");

	//***����������***//

	static bool isWork = true;   //���\���� ���������
	int inputLayerSize = 3;      //������ �������� ����
	int hiddenLayerSize = 2;     //������ �������� ����
	int outputLayerSize = 3;     //������ ��������� ����
	int getKey;                  //����� ������������ (����)
	double *actInput;            //������� ����
	double *actOutput;           //��������� �������� ���� 
	int realEpoch = 0;           //�������� ����� ������ � �����         

	//***���������***//

	cout << "\t\t\t�������� ��������\n";
	while (isWork) 
	{
		cout << "0. create Network\n";
		cout << "1. get vector\n";
		cout << "2. teach Network\n";
		cout << "3. delete Network\n";
		cout << "4. Exit\n";
		
		getKey = _getch();          //�������� ������ ������������
		switch (getKey) 
		{
		case 48:         //�� ������� 0
			createNetwork(inputLayerSize, hiddenLayerSize, outputLayerSize);
			break;       
			             
		case 49:         //�� ������� 1
			newNet = new network(inputLayerSize, hiddenLayerSize, outputLayerSize, 0); //������� ������ ���������
			init(inputLayerSize, hiddenLayerSize, outputLayerSize);                    //�������������� ���������
			actInput = new double[inputLayerSize];                                     //�������� ������ �������� ����
			cout << "Enter input vector with " << inputLayerSize << " values: ";       
			for (int i = 0; i < inputLayerSize; i++)                                   //���� �������� ����
			{
				cin >> actInput[i];
			}
			newNet->getActualOutput(actInput);     //�������� �������� ���� 
			delete newNet;                         //������� ������
			break;
		case 50:         //�� ������� 2
			newNet = new network(inputLayerSize, hiddenLayerSize, outputLayerSize, 0.4f, realEpoch); //������� ������ ���������
			init(inputLayerSize, hiddenLayerSize, outputLayerSize);                          //�������������� ���������

			actInput = new double[inputLayerSize];                                           //�������� ������ �������� ����
			actOutput = new double [outputLayerSize];                                        //�������� ������ ��������� ����

			cout << "Enter input vector with " << inputLayerSize << " values: ";
			for (int i = 0; i < inputLayerSize; i++)                                   //���� �������� ����
			{
				cin >> actInput[i];
			}

			cout << "Enter output vector with " <<outputLayerSize << " values: ";
			for (int i = 0; i < outputLayerSize; i++)                                  //���� ��������� ����
			{
				cin >> actOutput[i];
			}
			//actInput = new double[inputLayerSize] {1, 0, 1};   //������
			//actOutput = new double[outputLayerSize] {0, 1, 1};


				if (newNet->train(actInput, actOutput) == 100)                  //���������� ���������
				{                                                               //���� �������
					realEpoch = newNet->epoch;
					rewrite(inputLayerSize, hiddenLayerSize, outputLayerSize);  //�� �������������� ����� �����
					cout << "trainig completed! \n";
				}
				else
				{
					cout << "training was failed" << endl;
				}
			
			delete newNet;     //������� ������
			break;

		case 51:              //�� ������� 3
			deleteNetwork();  //������� ���������
			break;
		case 52:              //�� ������� 4
			isWork = false;   //��������� ���������
			break;
		default:
			break;
		}
		cout <<"_____________"<< endl << endl;
	}
	system("pause");
	return 0;               //����� �� ���������
}

//i-������ �������� ����
//h-������ �������� ����
//o-������ ��������� ����
int createNetwork(int i, int h, int o)   
{
	//***����������***//

	int _size1 = i * h;    //���-�� ����� � 1 �����
	int _size2 = h * o;    //���-�� ����� �� 2 �����
	double randVal;        //���������� �� ����. ���������
	std::default_random_engine ranEngine;
	std::uniform_real_distribution<double> dist(-0.3, 0.3);
	ofstream fout1;       //����� ������ � ���� 1
	ofstream fout2;       //����� ������ � ���� 2

	//***���������***//

	fout1.open(filePathInput_Hidden);  //��������� ���� 1
	fout2.open(filePathHidden_Output); //��������� ���� 2

	    //�������� ��������
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

		for (int j = 0; j < _size1; j++)  //���������� � ���� 1
		{
			randVal = dist(ranEngine);    //�������� ����. ��������
			if(j == _size1 - 1)
			fout1 << randVal;
			else 
			fout1 << randVal <<" ";
		}
		for (int j = 0; j < _size2; j++)  //���������� � ���� 1
		{
			randVal = dist(ranEngine);    //�������� ����. ��������
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
	ofstream fout;     //��������� ����� ��� ����������
	fout.open(filePathInput_Hidden);
	fout.close();
	fout.open(filePathHidden_Output);
	fout.close();
	fout.clear();
	cout << "\nNetwork was removed!\n";
	return 30;
}


//inp-������ �������� ����
//hid-������ �������� ����
//otp-������ ��������� ����
int init(int inp, int hid, int otp)
{

	//***����������***//
	ifstream reader1;
	ifstream reader2;

	//***���������***//

	reader1.open(filePathInput_Hidden);
	reader2.open(filePathHidden_Output);

	//�������� ��������
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
	if (reader1.eof() || reader2.eof())  //������ �����
	{
		cout << "files are empty: " << filePathInput_Hidden << endl;
		return 43;
	}

	//������� ������������ ������� ����� 1
	newNet->ihConnect = new double *[hid];
	for (int i = 0; i < hid; i++) 
	{
		newNet->ihConnect[i] = new double[inp];
	}

	//������� ������������ ������� ����� 2
	newNet->hoConnect = new double *[otp];
	for (int i = 0; i < otp; i++) 
	{
		newNet->hoConnect[i] = new double[hid];
	}


	//��������� � ����� 1 �������� � ���������� � ������� ����� 1
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

	//��������� � ����� 1 �������� � ���������� � ������� ����� 1
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


//inp-������ �������� ����
//hid-������ �������� ����
//otp-������ ��������� ����
int rewrite(int inp, int hid, int otp)
{

    //***����������***//

	double val;         //�������� �������� ��������
	ofstream rewrite_1; //����� ������ � ���� 1	
	ofstream rewrite_2; //����� ������ � ���� 2	

	//***���������***//

	cout << "\n\n\tNew weights: \n";
	cout << "Input-hidden: \n";
	rewrite_2.open(filePathHidden_Output);
	rewrite_1.open(filePathInput_Hidden);

	//��������� � ������ ����� 1 ���� � ���������� � ���� 1
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

	//��������� � ������ ����� 1 ���� � ���������� � ���� 1
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