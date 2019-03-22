#pragma once
#include <vector>
#include  <iostream>
#include <algorithm>
#include <chrono>
#include <windows.h>
#include <iomanip>

class TSP_G
{
	int N; //Liczba wierzcho³ków w grafie = matrix size
	int **matrix_distance;

	int iterations;
	int population_size;
	float mutation_probability;
	float crossover_probability;
	int mutation_manner;
	int crossover_manner;

	double max_time_s;

	//Najlepszy
	int best_distance;

	//Grupy
	std::vector<std::vector<int>> population;
	std::vector<std::vector<int>> best;
	std::vector<std::vector<int>> new_population;
	std::vector<float> fitness;
	std::vector<int> best_member;

	//Metody
	void create_basic_population();
	void shuffle(std::vector<int> &population_member, int num);
	void create_new_generation();
	float calculate_fitness(std::vector<int> population_member);

	//mutation
	void try_mutate(std::vector<int> &population_member);
	void transposition_mutation(std::vector<int> &permutation);
	void inversion_mutation(std::vector<int> &permutation);

	int calculate_cost(std::vector<int>& permutation);

	//krzyzowanie
	void cross_OX();
	void cross_PMX();

	void display_population();


	long long int read_QPC();
	long long int frequency, start, elapsed;	

public:
	TSP_G(int number_of_vertices, int** A, int population_size);
	void get_data(int iterations,	float mutation_probability,	int mutation_manner, int crossover_manner, float crossover_probability, double max_time);

	void solve();
	int get_cost() { return best_distance; }
	void display_results();

	~TSP_G();
};

