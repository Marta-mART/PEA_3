#include "TSP_G.h"




float TSP_G::calculate_fitness(std::vector<int> population_member)
{
	return (1.0f / (calculate_cost(population_member) + 1));
}


void TSP_G::create_basic_population()
{
	std::vector<int> population_member(N);

	//Wype³niamy pierwszego cz³onka populacji jako 0,1,2,3...
	for (int i = 0; i < N; ++i)
	{
		population_member[i] = i;
	}

	for (int i = 0; i < population_size; ++i)
	{
		//shuffle(population_member, 200);
		population.push_back(population_member);
	}
}

void TSP_G::shuffle(std::vector<int> &population_member, int num)
{
	//num - ile razy zamiana losowa ma siê dokonaæ
	for (int i = 0; i < num; i++)
	{
		//dwie losowe wartoœci ró¿ne od zera
		int random_city = rand() % (N - 1) + 1;
		int another_random_city = rand() % (N - 1) + 1;

		while (random_city == another_random_city)
		{
			another_random_city = rand() % (N - 1) + 1;
		}

		// Zamieñ miasta w jednej permutacji
		std::swap(population_member[random_city], population_member[another_random_city]);
	}
}

void TSP_G::try_mutate(std::vector<int> &population_member)
{
	if ((float)rand() / (float)RAND_MAX <= mutation_probability)
	{
		//std::cout << "\nmutacja\n";
		if(mutation_manner = 0) transposition_mutation(population_member);
		else inversion_mutation(population_member);
	}
		
}

void TSP_G::transposition_mutation(std::vector<int> &permutation)
{
	int index_1 = 1 + rand() % (N - 1);
	int index_2 = 1 + rand() % (N - 1);

	while (index_1 == index_2){
		index_2 = 1 + rand() % (N - 1);
	}

	std::swap(permutation[index_1],permutation[index_2]);
}

void TSP_G::inversion_mutation(std::vector<int>& permutation)
{
	int index_1, index_2;

	index_1 = 1 + rand() % (N - 1);
	index_2 = 1 + rand() % (N - 1);

	while (index_1 == index_2)
		index_2 = 1 + rand() % (N - 1);

	if (index_1 > index_2) 
		std::swap(index_1, index_2);

	//zaokraglenie w dol 
	int iterations_loop = ceil((index_2 - index_1) / 2.0f);

	//zamieniamy pierwszy element z ostatnim w podci¹gu
	//drugi z przedostatnim itd.
	for (int i = index_1, j=index_2; i < index_1 + iterations_loop; i++, j--)
	{
		std::swap(permutation[i], permutation[j]);
	}
}


void TSP_G::cross_OX()
{
	int index_p1, index_p2;

	index_p1 = (rand() % population_size);	
	index_p2= (rand() % population_size);
		
	while (index_p1 == index_p2)
		index_p2 = rand() % population_size;

	std::vector<int> parent1 = population.at(index_p1);
	std::vector<int> parent2 = population.at(index_p2);

	if ((float)rand() / (float)RAND_MAX > crossover_probability)
	{
		//dodaj rodziców, nie bêdzie dzieci
		new_population.push_back(parent1);
		new_population.push_back(parent2);
		return;
	}

	std::vector<int> child1(N);
	std::vector<int> child2(N);

	//Wybranie punktów krzy¿owania 
	int cross_point1 = 1;
	int cross_point2 = 1;

	while (cross_point1 == cross_point2)
	{
		cross_point1 = 1+ rand() % (N-1);
		cross_point2 = 1+ rand() % (N-1);
	}

	// Pierwszy punkt krzy¿owania nie mo¿e byæ po drugim
	// Jeœli tak siê stanie, zamieniamy je miejscami
	if (cross_point1 > cross_point2)
	{
		int temp = cross_point2;
		cross_point2 = cross_point1;
		cross_point1 = temp;
	}

	//METODA OX
	child1.at(0) = 0;
	for (int i = cross_point1; i <= cross_point2; ++i)
	{
		child1[i] = parent1[i];
		child2[i] = parent2[i]; 
	}

	//----operacja dla 1 dziecka
	for (int i = 1; i < cross_point1; i++)
	{
		for (int j = 1; j < N; j++)
		{
			if(std::find(child1.begin(), child1.end(), parent2.at(j)) != child1.end()) continue;
			else child1.at(i) = parent2.at(j);
		}			
	}

	for (int i = cross_point2+1; i < N; i++)
	{
		for (int j = 1; j < N; j++)
		{
			if (std::find(child1.begin(), child1.end(), parent2.at(j)) != child1.end()) continue;
			else child1.at(i) = parent2.at(j);
		}
	}
	//próba mutacji pierwszego dziecka
	try_mutate(child1);
	//Dodanie pierwszego dziecka powsta³ego z pary rodziców
	new_population.push_back(child1);


	//----operacja dla 2 dziecka
	for (int i = 1; i < cross_point1; i++)
	{
		for (int j = 1; j < N; j++)
		{
			if (std::find(child2.begin(), child2.end(), parent1.at(j)) != child2.end()) continue;
			else child2.at(i) = parent1.at(j);
		}
	}

	for (int i = cross_point2+1; i < N; i++)
	{
		for (int j = 1; j < N; j++)
		{
			if (std::find(child2.begin(), child2.end(), parent1.at(j)) != child2.end()) continue;
			else child2.at(i) = parent1.at(j);
		}
	}

	//próba mutacji pierwszego dziecka
	try_mutate(child2);

	//Dodanie pierwszego dziecka powsta³ego z pary rodziców
	new_population.push_back(child2);
	
	//powsta³o pierwsze dziecko
	/*std::cout << "C1: " << cross_point1 << std::endl;
	std::cout << "C2: " << cross_point2 << std::endl;
	std::cout << std::endl;
	std::cout << "P1: ";
	for (int i = 0; i < N; ++i)
		std::cout << parent1.at(i) << " ";
	std::cout << std::endl;
	std::cout << "P2: ";
	for (int i = 0; i < N; ++i)
		std::cout << parent2.at(i) << " ";
	std::cout << std::endl;
	std::cout << "child 1 : ";
		for (int i = 0; i < N; ++i) 
			std::cout << child1.at(i) << " ";
	std::cout << std::endl;
	std::cout << "child 2 : ";
	for (int i = 0; i < N; ++i)
		std::cout << child2.at(i) << " ";
	std::cout << std::endl;*/

}

//TODO - bad function!!! TODO
void TSP_G::cross_PMX()
{
	int index_p1, index_p2;

	index_p1 = (rand() % population_size);
	index_p2 = (rand() % population_size);

	while (index_p1 == index_p2)
		index_p2 = rand() % population_size;

	std::vector<int> parent1 = population.at(index_p1);
	std::vector<int> parent2 = population.at(index_p2);

	if ((float)rand() / (float)RAND_MAX > crossover_probability)
	{
		//dodaj rodziców, nie bêdzie dzieci
		new_population.push_back(parent1);
		new_population.push_back(parent2);
		return;
	}

	std::vector<int> child1(N);
	std::vector<int> child2(N);

	//Wybranie punktów krzy¿owania 
	int cross_point1 = 1;
	int cross_point2 = 1;

	while (cross_point1 == cross_point2)
	{
		cross_point1 = 1+ rand() % (N-1);
		cross_point2 = 1+ rand() % (N-1);
	}

	// Pierwszy punkt krzy¿owania nie mo¿e byæ po drugim
	// Jeœli tak siê stanie, zamieniamy je miejscami
	if (cross_point1 > cross_point2)
	{
		int temp = cross_point2;
		cross_point2 = cross_point1;
		cross_point1 = temp;
	}

	for (int i = 0; i < N; i++)
	{
		child1[i] = parent1[i];
		child2[i] = parent2[i];
	}

	//METODA PMX
	//std::vector<int>  swapped (N);
	//for (int i = cross_point1; i <= cross_point2; i++) 
	//{
	//	child1[i] = parent1[i];
	//	child2[i] = parent2[i];
	//	swapped[parent1[i]] = 1;
	//	swapped[parent2[i]] = 1;
	//}

	//child1.at(0) = 0;
	//// Druga pêtla krzy¿uj¹ca metod¹ PMX
	//// Zamienia pozosta³e elementy, aby unikn¹æ niespójnych permutacji
	//int swap_index = 0;
	//for (int i = 1; i < N; i++) {
	//	if (swapped[parent1[i]] != 1 || swapped[parent2[i]] != 1) {
	//		if (swap_index == cross_point1){
	//			swap_index = cross_point2;
	//		}

	//		child1[swap_index] = parent2[i];
	//		child2[swap_index] = parent1[i];
	//		swap_index++;
	//	}
	//}


	new_population.push_back(child1);
	new_population.push_back(child2);

	//powsta³o pierwsze dziecko
	/*std::cout << "C1: " << cross_point1 << std::endl;
	std::cout << "C2: " << cross_point2 << std::endl;
	std::cout << std::endl;
	std::cout << "P1: ";
	for (int i = 0; i < N; ++i)
		std::cout << parent1.at(i) << " ";
	std::cout << std::endl;
	std::cout << "P2: ";
	for (int i = 0; i < N; ++i)
		std::cout << parent2.at(i) << " ";
	std::cout << std::endl;
	std::cout << "child 1 : ";
	for (int i = 0; i < N; ++i)
		std::cout << child1.at(i) << " ";
	std::cout << std::endl;
	std::cout << "child 2 : ";
	for (int i = 0; i < N; ++i)
		std::cout << child2.at(i) << " ";
	std::cout << std::endl;*/
}

void TSP_G::create_new_generation()
{
	//czyœcimy now¹ populacjê, bo ju¿ zapisaliœmy najlepszych, reszta umiera
	new_population.clear();
	best.clear();
	//Nowa generacja bêdzie zawiera³a 2*n przedstawicieli (n starych rodziców + n nowych dzieci)

	//0. Krzyzowanie rodzicow - powstanie n dzieci
	for (int i = 0; i < population_size; i++)
	{		
		//	if (crossover_manner == 0) cross_PMX();
			//else
				cross_OX();	
	}
	
	//Koniecznie trzeba wywolac przed liczeniem fitness dla poszczególnych cz³onków! - musimy mieæ sumê
	//calculate_fitness(new_population);
	//1. Trzeba wybrac z nich n osobników najlepiej przystosowanych
	//a) Najpierw ich posortowaæ

	std::sort(new_population.begin(), new_population.end(), [this](auto i, auto j)->bool {return calculate_fitness(i) > calculate_fitness(j); });

	//Wielkoœæ nowej populacji to tak naprawdê population_size * 2
	int new_population_size = new_population.size();

	//-------------------------------------
	//Wyswietlenie wynikow
	//-------------------------------------
	//for (int i = 0; i < new_population_size; ++i)
	//{
	//	for (int j = 0; j < N; ++j)
	//	{
	//		std::cout << new_population.at(i).at(j) << " ";
	//	}
	//	std::cout << "cost : " << calculate_cost(new_population.at(i)) << std::endl;
	//}

	//b) wybranie pierwszych 50 jako tych najlepszych
	for (int i = 0; i < population_size; i++)
		best.push_back(new_population.at(i));

	//Populacjê stanowi¹ najlepsi
	population = best;
		
	//-------------------------------------
	//Wyswietlenie wynikow populacji wybranych osobników - nowa generacja
	//-------------------------------------
	//std::cout << "------------------------BEST --- to samo naj00000000---------------------------";
	//for (int i = 0; i < population_size; ++i)
	//{
	//	for (int j = 0; j < N; ++j)
	//	{
	//		std::cout << best.at(i).at(j) << " ";
	//	}
	//	std::cout << "cost : " << calculate_cost(best.at(i)) << std::endl;
	//}

}

void TSP_G::display_population()
{
	for (unsigned i = 0; i < population_size; i++)
	{
		for (unsigned j = 0; j < N; j++)
		{
			std::cout << population.at(i).at(j) << " ";
		}
		std::cout << std::endl;
	}
		
}

void TSP_G::display_results()
{
	std::cout << "best distance : " << best_distance << std::endl;
	std::cout << "population member : \n";
	for (int j = 0; j < N; j++) std::cout << best_member.at(j) << " ";
	std::cout << std::endl;
}

long long int TSP_G::read_QPC()
{
	LARGE_INTEGER count;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&count);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return((long long int)count.QuadPart);
}


TSP_G::TSP_G(int number_of_vertices, int ** A, int population_size)
{
	fitness.reserve(population_size);
	best_member.reserve(number_of_vertices);

	population.reserve(N);
	best.reserve(N);
	new_population.reserve(2 * N);

	this->population_size = population_size;
	N = number_of_vertices;

	//Kopiowanie macierzy A do macierzy matrix_distance
	// Wierszy tyle, ile wierzcholkow
	matrix_distance = new int *[N];

	// Kolumn tyle, ile wierzcho³ków
	for (int i = 0; i < N; i++) matrix_distance[i] = new int[N];

	// Wczytanie macierzy, ale zast¹pienie 0 na przek¹tnej -1
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			matrix_distance[i][j] = A[i][j];
			if (i == j)	matrix_distance[i][j] = 0;
		}

	best_distance = INT_MAX;

}

void TSP_G::get_data(int iterations, float mutation_probability, int mutation_manner, int crossover_manner, float crossover_probability, double max_time)
{
	this->iterations = iterations;
	if(mutation_probability <= 1) this->mutation_probability = mutation_probability;
	else mutation_probability = 0.05;
	if (mutation_probability > 0 && mutation_probability <= 1) this->mutation_manner = mutation_manner;
	else mutation_probability = 0.5;
	this->crossover_manner = crossover_manner;
	if(crossover_probability > 0 && crossover_probability <=1) this->crossover_probability = crossover_probability;
	else crossover_probability = 0.9;
	this->max_time_s = max_time;
}

void TSP_G::solve()
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
	//start zliczania czasu
	double elapsed_time = 0;

	//Wygenerowanie pocz¹tkowej permutacji - losowo
	create_basic_population();

	//Start czas (do warunku stopu)
	start = read_QPC();
	for (int i = 0; i < iterations; ++i)
	{
		//std::cout << "it: " << i << std::endl;
		if (elapsed_time >= max_time_s * 1000000) {
			//std::cout << "Warunek stopu: Przekroczono czas" << std::endl;
			break;
		}

		create_new_generation(); //tu jest cross i mutate

		int best_member_cost = calculate_cost(population.front());
		if (best_member_cost < best_distance)
		{
			best_distance = best_member_cost;
			best_member = population.front();			
		}

		/*std::cout << "best distance:" << best_distance << std::endl;
		std::cout << "population member\n";
			for (int j = 0; j < N; j++) std::cout << best_member.at(j) << " ";
				std::cout << std::endl;*/

		//Podliczenie, ile czasu trwa³o
		elapsed_time = read_QPC() - start;
	}
}

int TSP_G::calculate_cost(std::vector<int>& permutation)
{
	//pocz¹tkowy koszt drogi
	int cost = 0;

	for (int i = 0; i < N - 1; i++)
	{
		cost += matrix_distance[permutation.at(i)][permutation.at(i + 1)];
	}

	//dodajemy koszt trasy o koszt powrotu do wierzcho³ka 0
	cost += matrix_distance[permutation.at(N - 1)][0];
	return cost;
}

TSP_G::~TSP_G()
{
	//Zwolnienie pamiêci
	for (int i = 0; i < N; i++)
	{
		delete[] matrix_distance[i];
	}

	if (matrix_distance) delete[] matrix_distance;
}

