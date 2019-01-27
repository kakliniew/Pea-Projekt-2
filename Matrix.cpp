#include "pch.h"
#include "Matrix.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

Matrix::Matrix()
{
	size = 0;
}







Matrix::~Matrix()
{
	for (int i = 0; i < size; i++)
	{
		delete[] table[i];
	}
	delete[] table;
}

void Matrix::loadMatrix(string name)
{
	ifstream plik;
	plik.open(name, ios::in);
	if (plik.good() == true)
	{
		plik >> size;
		table = new int *[size];
		for (int i = 0; i < size; i++)
		{
			table[i] = new int[size];
		}

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				plik >> table[i][j];

			}


		}

		plik.close();
	}
	else cout << "Nie udalo sie zaladowac pliku" << endl;

}

void Matrix::displayMatrix()
{
	for (int i = 0; i < size; i++)
	{

		for (int j = 0; j < size; j++)
		{
			cout << table[i][j] << "\t";

		}
		cout << endl;
	}
	cout << endl;

}

int Matrix::getSize()
{
	return size;
}

void Matrix::createNeighborhoodMatrix()
{

	neighborhoodMatrix = new int *[size];
	for (int i = 0; i < size; i++)
	{
		neighborhoodMatrix[i] = new int[size];
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (table[i][j] <= 0)
			{
				neighborhoodMatrix[i][j] = 0;

			}
			else neighborhoodMatrix[i][j] = 1;

		}


	}
}

void Matrix::displayNeighborhoodMatrix()
{


	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << neighborhoodMatrix[i][j] << "\t";

		}
		cout << endl;


	}
}

int Matrix::GetPathLength(std::vector<int> route)
{
	int weightOfPath = 0;
	int licznik = 0;
	int licznik2 = 0;

	// indeksuje od 1, poniewaz ostatni element trzeba dodac na piechote
	for (int i =0; i < getSize()-1; i++)
	{
		// nie mozemy dopuscic do doliczenia do cyklu wartosc na przekatnej
		
			weightOfPath += table[route[i]][route[(i+1)%getSize()]];

		
		
	}
	weightOfPath += table[route[getSize()-1]][route[0]];
	return weightOfPath;
}


