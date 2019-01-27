// PEAp1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include "Matrix.h"
#include "Tabu.h"
#include "SimulatedAnnealing.h"
#include "Genetic.h"


using namespace std;

LARGE_INTEGER startTimer()
{
	LARGE_INTEGER start;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&start);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return start;
}
LARGE_INTEGER endTimer()
{
	LARGE_INTEGER stop;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&stop);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return stop;
}

int main()
{
	int wybor;
	bool work = true;
	int resultEnd;
	LARGE_INTEGER performanceCountStart, performanceCountEnd;
	int drogi = 0;
	
	do {

		cout << "Co chcesz zrobic?" << endl;
		cout << "1.Rozwiazac problem przy pomocy tabuSearch" << endl;
		cout << "2.Rozwiazac problem przy pomocy Simulated Annealing" << endl;
		cout << "3.Rozwiazac problem przy pomocy genetycznego" << endl;
		cout << "4.Zakonczyc dzialanie programu" << endl;
		cin >> wybor;

		switch (wybor)
		{
		case 1:
		{
			drogi = 0;
			Tabu tabu;
			tabu.loadMatrix("karol.txt");
			performanceCountStart = startTimer();
			for (int i = 0; i < 50; i++)
			{
				drogi+= tabu.tabuTSP(21, 3000);
			}
			performanceCountEnd = endTimer();
			double tm = performanceCountEnd.QuadPart - performanceCountStart.QuadPart;
			cout << "Sredni czas : " << tm/(2850*50) << " ms" << endl;
			cout << "Sredni koszt : " << drogi/50  << endl; break;


		}

		case 2:
		{
			drogi = 0;
			SimulatedAnnealing simu;
			simu.loadMatrix("karol.txt");
			performanceCountStart = startTimer();
			for (int i = 0; i < 50; i++)
			{
			simu.TSPp2(1000000000, 0.999); 
			drogi += simu.getCost();
			}
			performanceCountEnd = endTimer();
			//for (auto it = simu.bestRoute.begin(); it != simu.bestRoute.end(); ++it) cout << *it<<endl;
			cout << "Sredni koszt: " << drogi/50 << endl;
			
			double tm = performanceCountEnd.QuadPart - performanceCountStart.QuadPart;
			cout << "Czas : " << tm / (2850*50) << " ms" << endl; break;
		}

		case 3:
		{	drogi = 0;
			Genetic genetic;
			genetic.loadMatrix("karol.txt");

			performanceCountStart = startTimer();
			for (int i = 0; i < 50; i++)
			{
				drogi += genetic.GeneticTSP(10,4, 0.5, false);
			}
			performanceCountEnd = endTimer();
			cout << "Sredni koszt: " << drogi / 50 << endl;

			double tm = performanceCountEnd.QuadPart - performanceCountStart.QuadPart;
			cout << "Czas : " << tm / (2850 * 50) << " ms" << endl; break;
			
			
		}

		case 4:
		{
			work = false; break;
		}

		default: cout << "Bledny numer" << endl; break;

		}





	} while (work);








}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
