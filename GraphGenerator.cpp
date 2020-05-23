#include <iostream>
#include <time.h>
#include <list>
#include <fstream>

struct Edge {
	const unsigned int U, V;
	const int W;

	Edge(const unsigned int u, const unsigned int v, const int w) : U(u), V(v), W(w) {}
	~Edge() {}
	const bool operator<(const Edge& rhs) const {
		return U < rhs.U || U == rhs.U && V < rhs.V;
	}
};

const int randomValue(const int& min, const int& max) {
	return (rand() % ((max - min) + 1)) + min;
}

const bool isToInsert(const float& probability) {
	return (rand() % 100) < probability;
}

std::list<Edge>* randow(const unsigned int& amountVertices, const int& isValued, const float& probability) {
	std::list<Edge>* edges = new std::list<Edge>();

	for (unsigned int u = 1; u <= amountVertices; u++)
		for (unsigned int v = 1; v <= amountVertices; v++)
			if (isToInsert(probability))
				edges->emplace_back(u, v, randomValue(-isValued, isValued));

	return edges;
}

std::list<Edge>* simpleRandow(const unsigned int& amountVertices, const bool& isDigraph, const int& isValued, const float& probability) {
	int* matrix = new int[amountVertices * amountVertices];
	for (unsigned int i = 0; i < amountVertices * amountVertices; i++)
		matrix[i] = 0;

	for (unsigned int u = 0; u < amountVertices; u++)
		for (unsigned int v = 0; v < amountVertices; v++)
			if (u != v) {
				if (isToInsert(probability))
					if (v < u && !matrix[u * amountVertices + v])
						matrix[v * amountVertices + u] = randomValue(-isValued, isValued);
					else
						matrix[u * amountVertices + v] = randomValue(-isValued, isValued);
			}

	std::list<Edge>* edges = new std::list<Edge>();

	for (unsigned int u = 0; u < amountVertices; u++)
		for (unsigned int v = 0; v < amountVertices; v++)
			if (matrix[u * amountVertices + v])
				edges->emplace_back(u + 1, v + 1, matrix[u * amountVertices + v]);

	delete[] matrix;
	matrix = nullptr;
	return edges;
}

std::list<Edge>* completeRegular(const unsigned int& amountVertices, const bool& isDigraph, const int& isValued) {
	std::list<Edge>* edges = new std::list<Edge>();

	for (unsigned int u = 1; u <= amountVertices; u++)
		for (unsigned int v = u + 1; v <= amountVertices; v++) {
			edges->emplace_back(u, v, randomValue(-isValued, isValued));
			if (isDigraph)
				edges->emplace_back(v, u, randomValue(-isValued, isValued));
		}
	return edges;
}

std::list<Edge>* completeBipartite(const unsigned int& amountVerticesA, const unsigned int& amountVerticesB, const int& isValued) {
	const unsigned int amountVertices = amountVerticesA + amountVerticesB;
	std::list<Edge>* edges = new std::list<Edge>();

	for (unsigned int u = 1; u <= amountVerticesB; u++)
		for (unsigned int v = 1; v <= amountVerticesA; v++)
			edges->emplace_back(u, v + amountVerticesB, randomValue(-isValued, isValued));

	return edges;
}

int main() {
	srand(time(NULL));

	int value = 0;
	do {
		std::cout << "Dgt 1: Novo Grafo aleatorio" << std::endl;
		std::cout << "Dgt 2: Novo Grafo aleatorio simples" << std::endl;
		std::cout << "Dgt 3: Novo grafo regular completo" << std::endl;
		std::cout << "Dgt 4: Novo grafo bipartido completo" << std::endl;
		std::cout << "Dgt 0: Encerrar." << std::endl;
		std::cin >> value;
	} while (value < 0 || value > 4);

	if (value > 0) {
		std::list<Edge>* edges = nullptr;
		unsigned int amountVertices;
		int isValued;
		bool isDigraph;

		if (value == 4) {
			std::cout << "Dgt a qtd de Vertices do conjunto 1." << std::endl;
			unsigned int amountVerticesA;
			std::cin >> amountVerticesA;

			std::cout << "Dgt a qtd de Vertices do conjunto 2." << std::endl;
			unsigned int amountVerticesB;
			std::cin >> amountVerticesB;

			std::cout << "Dgt 0: Grafo nao valorado.\nDgt o maior valor : Grafo valorado." << std::endl;
			std::cin >> isValued;
			isValued = abs(isValued);

			isDigraph = false;

			edges = completeBipartite(amountVerticesA, amountVerticesB, isValued);

			amountVertices = amountVerticesA + amountVerticesB;
		} else {
			std::cout << "Dgt a qtd de Vertices." << std::endl;
			std::cin >> amountVertices;

			std::cout << "Dgt 0: Grafo nao valorado.\nDgt o maior valor : Grafo valorado." << std::endl;
			std::cin >> isValued;
			isValued = abs(isValued);

			std::cout << "Dgt 0: Grafo nao dirigido.\nDgt qualquer valor: Grafo dirigido." << std::endl;
			std::cin >> isDigraph;

			if (value == 1 || value == 2) {
				std::cout << "Dgt a probabilidade de insersao" << std::endl;
				float probability;
				std::cin >> probability;

				if (value == 1)
					edges = randow(amountVertices, isValued, probability);
				else
					edges = simpleRandow(amountVertices, isDigraph, isValued, probability);

			} else if (value == 3) {
				edges = completeRegular(amountVertices, isDigraph, isValued);
			}
		}

		if (edges != nullptr) {
			std::ofstream file;
			file.open("Graph.txt");
			file << amountVertices << " " << edges->size() << " " << (isDigraph ? 1 : 0) << std::endl;
			for (auto e = edges->cbegin(); e != edges->cend(); e = edges->erase(e))
				file << e->U << " " << e->V << " " << e->W << std::endl;
			file.close();
			delete edges;
			edges = nullptr;
		}
	}
	std::cout << "Programa encerrado";
	return 0;
}
