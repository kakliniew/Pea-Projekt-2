#include "pch.h"
#include "SimulatedAnnealing.h"
#include <iostream>


SimulatedAnnealing::SimulatedAnnealing()
{
	tempCost = 0;
	temperature = 0;
	tempCoefficient = 0;
}


SimulatedAnnealing::~SimulatedAnnealing()
{
}

void SimulatedAnnealing::Arithmetic()
{
	temperature -= tempCoefficient;
}

void SimulatedAnnealing::Geo()
{
	temperature *= tempCoefficient;
}
void SimulatedAnnealing::SetTemperature(float temp)
{
	temperature = temp;
}

float SimulatedAnnealing::GenerateProbability()
{
	float value = pow(e, (((float)(finalCost - tempCost)) / temperature));
	if (value < 1.0) return value;
	return 1.0f;
}

float SimulatedAnnealing::GenerateRandomProbability()
{
	float probability = (rand() % 10)/10;
	return probability;
}

void SimulatedAnnealing::Permute()
{
	int auxNumber;
	int indexOfFirstVertex = rand() % getSize();
	int indexOfSecondVertex;

	do
	{
		// jesli dwie wylosowane liczby sa te same powtorz losowanie drugiej liczby
		indexOfSecondVertex = rand() % getSize();
	} while (indexOfFirstVertex == indexOfSecondVertex);

	// przestawienie dwoch liczb w wektorze
	auxNumber = finalRoute[indexOfFirstVertex];
	tempRoute = finalRoute;
	tempRoute[indexOfFirstVertex] = tempRoute[indexOfSecondVertex];
	tempRoute[indexOfSecondVertex] = auxNumber;


}

void SimulatedAnnealing::TSPp2(float tMax, float tCoeff)
{
	tempCoefficient = tCoeff;
	temperature = tMax;
	

	int x = rand() % getSize();

	for (int i = x; i < getSize(); i++)
	{
		finalRoute.push_back(i);
	}
	for (int i = 0; i < x; i++)
	{
		finalRoute.push_back(i);
	}

	tempRoute = finalRoute;
	finalCost = GetPathLength(tempRoute);
	tempCost = finalCost;

	while (temperature > 0.0001)
	{

		Permute();
	    tempCost = GetPathLength(tempRoute);
		

		if (tempCost < finalCost)
		{
			
			finalCost = tempCost;
			finalRoute = tempRoute;
			bestRoute = finalRoute;
			bestRouteCost = finalCost;
		}
		else if (GenerateRandomProbability() < GenerateProbability())
		{
			finalCost = tempCost;
			finalRoute = tempRoute;
		}
		Geo();

	}

}

int SimulatedAnnealing::getCost()
{
	return bestRouteCost;
}


