#pragma once
#include "Matrix.h"
class Tabu :
	public Matrix
{

	int **tabu_list;
	int *best_solution;
	int *solution;
	int bestSolutionCost;
	int *now_solution;
	int nowSolutionCost;

public:
	Tabu();
	
	~Tabu();
	int tabuTSP(int, int);
	void updateTabu();
	void select();
	int calculateSolution(int*);
	
	int difference(int**, int*, int, int, bool);
	void wyswietl();

};

