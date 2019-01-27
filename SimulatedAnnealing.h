#pragma once
#include "Matrix.h"
#include <vector>

const double e = 2.718281828459045;
class SimulatedAnnealing :
	public Matrix
{
	float temperature;
	float minTemperature;
	float tempCost;
	float tempCoefficient;
	int bestRouteCost;
	int finalCost;
public:
	SimulatedAnnealing();
	~SimulatedAnnealing();
	void Arithmetic();
	void SetTemperature(float);
	float GenerateProbability();
	float GenerateRandomProbability();	
	void Permute();
	void TSPp2(float, float);
	int getCost();
	void Geo();
	
};

