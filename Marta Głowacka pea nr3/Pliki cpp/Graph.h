#pragma once
#include <iostream>
#include <iomanip> // setw
class Graph
{
public:
	int **A;			// macierz s�siedztwa
	int vertices_num;	// liczba wierzcho�k�w

	void display_graph();	//wyswietlenie grafu
	Graph(std::string file_name);
	Graph(int vN, int wageMin, int wageMax);
	~Graph();
	int ** get_matrix() { return A; }
	int getVertices() { return vertices_num; }
};


