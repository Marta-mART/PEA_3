#include <iostream>
#include <string>
#include <fstream>
#include <chrono> // czas
#include <iomanip>
#include <fstream>
#include <windows.h>
#include "Graph.h"
#include "TSP_G.h"

//-------------------------------------------------------
// PEA Marta G³owacka 10.01.2019
//-------------------------------------------------------

using namespace std;

//Zmienne globalne do pomiaru czasu
long long int frequency, start, elapsed;
long long int read_QPC();
void display_time();

int population_size = 50;
float iteration_criterion = 100;
float mutation_rate = 0.1f;
float crossover_rate = 0.90f;

int	mutation_manner = 0; //(0) Transpozycja (1) Inwersja
int crossover_manner = 0; //(0) PMX (1) OX

void give_stop_criterion();
void give_population_size();
void give_mutation_rate();
void give_crossover_rate();
void give_mutation_manner();
void give_crossover_manner();

void display_settings();
void test();

void main()
{
	srand(time(0));

	int choice;

	do
	{
		start = 0, elapsed = 0;
		system("cls");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		std::cout << std::endl;
		std::cout << "----------------" << std::endl;
		std::cout << "PROBLEM KOMIWOJAZERA - PROJEKT 3" << std::endl;
		std::cout << "----------------" << std::endl;

		// Zmiana koloru czcionki
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

		// Wyswietlenie opcji - wybór struktury
		std::cout << std::endl << "MENU: \n";
		std::cout << "-------------------------------\n";
		std::cout << "1. Wczytanie danych z pliku \n";
		std::cout << "2. Wprowadzenie kryterium stopu \n";
		std::cout << "3. Ustawienie wielkosci populacji poczatkowej \n";
		std::cout << "4. Ustawienie wspolczynnika mutacji \n";
		std::cout << "5. Ustawienie wspolczynnika krzyzowania \n";
		std::cout << "6. Wybor metody mutacji\n";
		std::cout << "7. Wybor metody krzyzowania\n";
		std::cout << "8. Uruchomienie algorytmu i wyswietlenie wynikow \n";
		std::cout << "9. Wyjscie \n";
		std::cout << "-------------------------------\n";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		cin >> choice;

		Graph myGraph("data17.txt");

		switch (choice)
		{
		case 1:
		{
			myGraph.display_graph();
		}
		break;
		case 2:
		{
			give_stop_criterion();
		}
		break;
		case 3:
		{
			give_population_size();
		}
		break;
		case 4:
		{
			give_mutation_rate();
		}
		break;
		case 5:
		{
			give_crossover_rate();
		}
		break;
		case 6:
		{
			give_mutation_manner();
		}
		break;
		case 7:
		{
			give_crossover_manner();
			//test();
		}
		break;
		case 8:
		{
			display_settings();
			TSP_G algorythm(myGraph.getVertices(), myGraph.get_matrix(), population_size);
			algorythm.get_data(iteration_criterion, mutation_rate, mutation_manner, crossover_manner, crossover_rate, 1000);
			start = read_QPC();			
			algorythm.solve();
			elapsed = read_QPC() - start;

			display_time();

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			algorythm.display_results();
		}
		break;
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		std::cout << "[Wcisnij ENTER, aby wybrac kolejna opcje]" << std::endl;

		cin.get();
		cin.get();


	} while (choice != 9);

}


void give_stop_criterion()
{
	cout << "Podaj max liczbe generacji (iteracji)	" << std::endl;
	cin >> iteration_criterion;
}
void give_population_size()
{
	cout << "Rozmiar populacji: ";
	cin >> population_size;
}
void give_mutation_rate()
{
	cout << "Wspolczynnik mutacji (0-1]: ";
	cin >> mutation_rate;
}
void give_crossover_rate()
{
	cout << "Wspolczynnik krzyzowania (0-1]: ";
	cin >> crossover_rate;
}
void give_mutation_manner()
{
	cout << "Wprowadz metodê mutacji (0) Transpozycja (1) Inwersja: ";
	cin >> mutation_manner;
}

void give_crossover_manner()
{
	cout << "Wprowadz metodê krzyzowania (0) PMX (1) OX: "; 
	cin >> crossover_manner; //(0) PMX (1) OX
}

//-------------------------------------------------------
//Funkcja do odczytu minionego czasu
//-------------------------------------------------------
long long int read_QPC()
{
	LARGE_INTEGER count;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&count);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return((long long int)count.QuadPart);
}


void display_time()
{
	// Zmiana koloru czcionki
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	std::cout << "Czas wykonania: " << elapsed << " us" << std::endl;
}

void display_settings()
{
	// Zmiana koloru czcionki
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	std::cout << "Kryterium stopu: " << iteration_criterion << " iteracji" << std::endl;
	std::cout << "Wielkosc populacji poczatkowej: " << population_size << " osobnikow" << std::endl;
	std::cout << "Wspolczynnik mutacji: " << mutation_rate << " /1" << std::endl;
	std::cout << "Wspolczynnik krzyzowania: " << crossover_rate << " /1" << std::endl;
	std::cout << "Metoda mutacji: ";
	if(mutation_manner == 0) std::cout << "transpozycja" << std::endl;
	if (mutation_manner == 1) std::cout << "inwersja" << std::endl;

}

void load_to_file_times(std::string file_name, double time)
{
	// Zmienna do wpisywania w pliku
	std::ofstream plik;

	// Otwieranie pliku
	plik.open(file_name.c_str(), std::ios::app);

	// Gdy blad otwarcia pliku
	if (plik.bad() || plik.fail())
	{
		cout << "Nie udalo sie otworzyc pliku.";
	}

	plik << time << endl;

}

void load_to_file_costs(std::string file_name, int cost)
{
	// Zmienna do wpisywania w pliku
	std::ofstream plik;

	// Otwieranie pliku
	plik.open(file_name.c_str(), std::ios::app);

	// Gdy blad otwarcia pliku
	if (plik.bad() || plik.fail())
	{
		cout << "Nie udalo sie otworzyc pliku.";
	}

	plik << cost << endl;

}

void test()
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);

	std::string tab_tsp[3] = { "data17.txt", "data56.txt", "data171.txt" };
	const int number_of_tests = 3;

	double criteria[7] = {
		1000, // population size
		100, // iterations
		0.01, // mutation probability
		1,//mutation manner 1 or 0,
		1, // crossover manner 1 or 0 (ox only available)
		0.7,// crossover  probability
		120}; //max time

	std::cout << "-------------------------------------------------" << std::endl;
	std::cout << "Instancja : " << tab_tsp[2];

	Graph test_graph(tab_tsp[2]);
	
	 
	//testy dla ró¿nej l. iteracji
	for (int j = 0; j < number_of_tests; j++)
	{
		TSP_G tsp_g(test_graph.getVertices(), test_graph.get_matrix(), criteria[0]);
		tsp_g.get_data(criteria[1], criteria[2], criteria[3], criteria[4], criteria[5], criteria[6]);
		//---Pomiar czasu
		start = read_QPC();
		tsp_g.solve();
		elapsed = read_QPC() - start;
		//---Koniec pomiaru czasu

		load_to_file_times("wyniki_pomiarow_GA_pop.txt", criteria[0]);
		load_to_file_costs("wyniki_pomiarow_GA_costs.txt", tsp_g.get_cost());

		std::cout << "time : " << criteria[6] << " elapsed : " << (float)elapsed /
			frequency << " cost : " << tsp_g.get_cost() << std::endl;


	}

}

