#include "Graph.h"
#include <fstream> 

//-------------------------------------------------------
//Konstruktor grafu
//-------------------------------------------------------
Graph::Graph(std::string file_name)
{
	//std::cout << "Konstruktor grafu" << std::endl;

	A = nullptr;

	// Zmienna do wczytywania z pliku
	std::ifstream plik;

	// Otwieranie pliku
	plik.open(file_name.c_str(), std::ios::in);

	// Gdy blad otwarcia pliku
	if (plik.bad() || plik.fail())
	{
		std::cout << "Nie udalo sie otworzyc pliku." << std::endl;
		exit(0);
	}

	plik >> vertices_num;

	// Stworzenie  dynamicznie macierzy kwadratowej
	// o wymiarze liczba wierzcho�k�w na liczba wierzcho�k�w

	// Wierszy tyle, ile wierzcholkow
	A = new int *[vertices_num];

	// Kolumn tyle, ile wierzcho�k�w
	for (int i = 0; i < vertices_num; i++) A[i] = new int[vertices_num];

	// Wczytanie macierzy
	for (int i = 0; i < vertices_num; i++)
	{
		for (int j = 0; j < vertices_num; j++)
		{
			plik >> A[i][j];
			//std::cout << A[i][j] << " ";
		}
		//std::cout << std::endl;
	}



	std::cout << std::endl;
}

//-------------------------------------------------------
//Konstruktor grafu do test�w
//-------------------------------------------------------
Graph::Graph(int vN, int wageMin, int wageMax)
{
	//std::cout << "Konstruktor 2 grafu" << std::endl;

	A = nullptr;

	vertices_num = vN;

	// Stworzenie  dynamicznie macierzy kwadratowej
	// o wymiarze liczba wierzcho�k�w na liczba wierzcho�k�w

	// Wierszy tyle, ile wierzcholkow
	A = new int *[vertices_num];

	// Kolumn tyle, ile wierzcho�k�w
	for (int i = 0; i < vertices_num; i++) A[i] = new int[vertices_num];

	int generated;

	// Wygenerowanie macierzy
	for (int i = 0; i < vertices_num; i++)
		for (int j = 0; j <= i; j++)
		{
			if (i == j) {
				A[i][j] = 0; continue;
			}

			//Macierz symetryczna
			generated = wageMin + rand() % wageMax + 1;
			A[i][j] = generated;
			A[j][i] = generated;
		}

}

//-------------------------------------------------------
//Destruktor grafu
//-------------------------------------------------------
Graph::~Graph()
{
	//Usuwamy macierz z pami�ci komputera
	for (int i = 0; i < vertices_num; i++)
	{
		if (A[i]) delete[] A[i];
	}

	if (A) delete[] A;

	std::cout << "Destruktor grafu" << std::endl;
}

//-------------------------------------------------------
//Wy�wietlenie wynik�w
//-------------------------------------------------------
void Graph::display_graph()
{
	std::cout << "Graf jako macierz:" << std::endl;
	for (int i = 0; i < vertices_num; i++)
	{
		std::cout << "v" << i << " ";
		for (int j = 0; j < vertices_num; j++)
		{
			std::cout << std::setw(3) << A[i][j];
			std::cout << " ";
		}
		std::cout << std::endl << std::endl;
	}
}