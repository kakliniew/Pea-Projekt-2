#pragma once
#include "Matrix.h"
class Genetic :
	public Matrix
{


	
public:
	Genetic();
	~Genetic();
	int GeneticTSP(int, int,double,bool);
	void opt_sciezka(int* kolejnosc, int*wynik, int a, int b);
	void quicksortUp(int *priorytet, int *kolejnosc, int lewy, int prawy);
	int wylicz_droge(int*kolejnosc);
	
	void wyswietl_tab(int*tab, int x);
};

