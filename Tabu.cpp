#include "pch.h"
#include "Tabu.h"
#include <iostream>
#include <algorithm>
#include <list>
#include <cstdlib>
#include <ctime>
Tabu::Tabu()
{
	best_solution = new int[getSize()];
	now_solution = new int[getSize()];
}




Tabu::~Tabu()
{
}

int Tabu::tabuTSP(int tabuListSize, int repeats)
{
	//if (this == NULL) return;
	solution = new int[getSize()];
	best_solution = new int[getSize()];
	//srand(time(NULL));



	////////////
	///////////  generoanie pierwszej sciezki
	////////////
	int droga, najlepsza, droga_temp;
	now_solution = new int[getSize()];
	int minim = 123456789;


	for (int m = 0; m < getSize(); m++)
	{
		solution[m] = m;
	}
	//losowanie pierwszego miasta
	int x = rand() % getSize();
	solution[0] = x;
	//std::cout << "solucja" << solution[0] << std::endl;
	solution[x] = 0;
	droga = 0;
	int minimum;
	int pozycja;
	
	////
	//// wlasciwa czesc algorytmu
	////
	int bestX, bestY;
	int aktualnaDroga, najlepszaDroga, aktualnaDrogaTemp;
	char znak;
	int najlepszaRoznicaDlugosci, roznicaDlugosci;

	std::pair<int, int> porownywana(-1, -1);
	std::list<std::pair<int, int>> listaTabu;
	std::list<std::pair<int, int>>::iterator wyswietlany;
	bool zapisz, nadpisz;

	for (int i = 0; i < getSize(); i++)
	{
		best_solution[i] = solution[i]; //zapisz pierwsze rozwiazanie
	}
	aktualnaDrogaTemp = aktualnaDroga = najlepszaDroga = calculateSolution(solution); //oblicz pierwsza droge
	/*std::cout << "Pierwsza wygenerowana droga: ";
	for (int i = 0; i < getSize(); i++)
	{
		std::cout << best_solution[i] << std::endl;
	}
	std::cout << "pierwsza obliczona droga: " << najlepszaDroga << std::endl;
	*/
	for (int i = 0; i < repeats; i++) //dla zadanej liczby powtorzen
	{

		nadpisz = false;
		najlepszaRoznicaDlugosci = INT_MIN;
		for (int i = 0; i < getSize(); i++) //szukanie najlepszej zamiany miast z sasiedztwa
		{
			for (int j = i + 1; j < getSize(); j++)
			{
				
				zapisz = false;
				roznicaDlugosci = difference(table, solution, i, j, true); //oblicz roznice dlugosci
				if (roznicaDlugosci > najlepszaRoznicaDlugosci) //jesli nowe najlepsze w sasiedztwie
				{
					//std::cout << "Nowe najlepszee rozwiazanie w sasiedztwie " << std::endl;
					if (aktualnaDroga - roznicaDlugosci < najlepszaDroga) //jesli najlepsze globalnie na pewno zapisz, nie sprawdzaj w liscie tabu
					{
						//std::cout << "Nowe najlepszee rozwiazanie globalnie " << std::endl;

						zapisz = true;
						nadpisz = true;
						//najlepszaDroga=aktualnaDroga-najlepszaRoznicaDlugosci;

					}
					else //jesli nie to sprawdz w liscie tabu, jesli nie ma to zapisz jako najlepsze, jesli jest to nic nie rob
					{
						porownywana.first = i;
						porownywana.second = j;
						if (std::find(listaTabu.begin(), listaTabu.end(), porownywana) == listaTabu.end()) zapisz = true;
						else
						{
							//std::cout << "Zamiana istnieje juz w liscie tabu" << std::endl;

						}
					}
					if (zapisz) //zapis najlepszego z sasiedztwa
					{
						najlepszaRoznicaDlugosci = roznicaDlugosci;
						bestX = i;
						bestY = j;
						aktualnaDrogaTemp = aktualnaDroga - najlepszaRoznicaDlugosci;
					}
				}
			}
		}

		//
		//aktualizowanie aktualnego i ew. najlepszego rozwiazania (jesli nadpisz==true)
		//

		solution[bestX] += solution[bestY];
		solution[bestY] = solution[bestX] - solution[bestY];
		solution[bestX] -= solution[bestY];
		aktualnaDroga = aktualnaDrogaTemp;
		
	/*	std::cout << "Nowe aktualne rozwiazanie: " << std::endl;
		for (int i = 0; i < getSize(); i++)
		{
			std::cout << solution[i] << std::endl;
		}
		std::cout << "Nowe aktualne dlugosc:  " <<aktualnaDroga<< std::endl;
		*/

		if (nadpisz)
		{
			
			best_solution[bestX] += best_solution[bestY];
			best_solution[bestY] = best_solution[bestX] - best_solution[bestY];
			best_solution[bestX] -= best_solution[bestY];
			najlepszaDroga = aktualnaDroga;
		}

		listaTabu.push_front(std::pair<int, int>(bestX, bestY)); //dodanie ostatnio zamienionej pary do listy tabu
		listaTabu.resize(tabuListSize); //przyciecie listy tabu do odpowiedniej dlugosci, usuniecie starych wpisow

		
		/*std::cout << "Lista Tabu " << std::endl;
		for (wyswietlany = listaTabu.begin(); wyswietlany != listaTabu.end(); wyswietlany++)
		{
			if (wyswietlany->first == wyswietlany->second) break;
			std::cout << "(" << wyswietlany->first << ", " << wyswietlany->second << ") ";
		}
		std::cout << std::endl;

		*/

	}
	
	
	return najlepszaDroga;
	
	
}

void Tabu::updateTabu()
{
}

void Tabu::select()
{
}

int Tabu::calculateSolution(int *solution)
{
	int droga = 0;
	for (int i = 0; i < getSize() - 1; i++)
	{
		droga += table[solution[i]][solution[i + 1]];

	}
	droga += table[solution[getSize() - 1]][solution[0]];
	return droga;
}



int Tabu::difference(int **table, int *solution, int i, int j, bool change)
{
	int wynik = 0;
	if (i == 0)
	{
		wynik -= (table[solution[getSize() - 1]][solution[0]] + table[solution[0]][solution[1]]);
	}
	else if (i == getSize() - 1)
	{
		wynik -= (table[solution[getSize() - 2]][solution[getSize() - 1]] + table[solution[getSize() - 1]][solution[0]]);
	}
	else wynik -= (table[solution[i - 1]][solution[i]] + table[solution[i]][solution[i + 1]]);


	if (j == 0)
	{
		if (i != getSize() - 1) wynik -= table[solution[getSize() - 1]][solution[0]];
		if (i != 1) wynik -= table[solution[0]][solution[1]];
	}
	else if (j == getSize() - 1)
	{
		if (i != getSize() - 2) wynik -= table[solution[getSize() - 2]][solution[getSize() - 1]];
		if (i != 0) wynik -= table[solution[getSize() - 1]][solution[0]];
	}
	else
	{
		if (i != j - 1) wynik -= table[solution[j - 1]][solution[j]];
		if (i != j + 1) wynik -= table[solution[j]][solution[j + 1]];
	}

	solution[i] += solution[j];
	solution[j] = solution[i] - solution[j];
	solution[i] -= solution[j];
	wynik *= -1;
	if (i == 0)
	{
		wynik -= (table[solution[getSize() - 1]][solution[0]] + table[solution[0]][solution[1]]);
	}
	else if (i == getSize() - 1)
	{
		wynik -= (table[solution[getSize() - 2]][solution[getSize() - 1]] + table[solution[getSize() - 1]][solution[0]]);
	}
	else wynik -= (table[solution[i - 1]][solution[i]] + table[solution[i]][solution[i + 1]]);


	if (j == 0)
	{
		if (i != getSize() - 1) wynik -= table[solution[getSize() - 1]][solution[0]];
		if (i != 1) wynik -= table[solution[0]][solution[1]];
	}
	else if (j == getSize() - 1)
	{
		if (i != getSize() - 2) wynik -= table[solution[getSize() - 2]][solution[getSize() - 1]];
		if (i != 0) wynik -= table[solution[getSize() - 1]][solution[0]];
	}
	else
	{
		if (i != j - 1) wynik -= table[solution[j - 1]][solution[j]];
		if (i != j + 1) wynik -= table[solution[j]][solution[j + 1]];
	}
	if (change)
	{
		solution[i] += solution[j];
		solution[j] = solution[i] - solution[j];
		solution[i] -= solution[j];
	}
	return wynik;
}

void Tabu::wyswietl()
{
	std::cout << "Najlepsze znalezione roziwazanie " << std::endl;
	for (int i = 0; i < getSize(); i++)
	{
		std::cout << best_solution[i] << std::endl;
	}
	
}
