#pragma once
#include <string>
#include <vector>
class Matrix
{
	int size;
public:
	int **table;
	int **neighborhoodMatrix;
	std::vector<int> finalRoute;
	std::vector<int> tempRoute;
	std::vector<int> bestRoute;
	Matrix();
	
	~Matrix();
	void loadMatrix(std::string);
	void displayMatrix();
	int getSize();
	void createNeighborhoodMatrix();
	void displayNeighborhoodMatrix();
	int GetPathLength(std::vector<int>);

};

