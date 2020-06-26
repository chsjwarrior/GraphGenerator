#include <iostream>
#include <time.h>
#include <list>
#include <fstream>
#include <vector>

struct Edge {
	const unsigned int U, V;
	const int W;

	Edge(const unsigned int u, const unsigned int v, const int w) : U(u), V(v), W(w) {}
	~Edge() {}
	const bool operator>(const Edge& other) const {
		if (U > other.U) return true;
		if (other.U > U) return false;
		if (V > other.V) return true;
		if (other.V > V) return false;
		if (W > other.W) return true;
		if (other.W > W) return false;
		return false;
	}
};

inline const int randomValue(const int& min, const int& max) {
	return (rand() % ((max - min) + 1)) + min;
}

inline const bool isToInsert(const float& probability) {
	return (rand() % 100) < probability;
}

std::list<Edge>* randow(const unsigned int& amountVertices, const int& maxValue, const int& minValue, const float& probability) {
	std::list<Edge>* edges = new std::list<Edge>();

	for (unsigned int u = 1; u <= amountVertices; ++u)
		for (unsigned int v = 1; v <= amountVertices; ++v)
			if (isToInsert(probability))
				edges->emplace_back(u, v, randomValue(minValue, maxValue));

	return edges;
}

std::list<Edge>* simpleRandow(const unsigned int& amountVertices, const bool& isDigraph, const int& maxValue, const int& minValue, const float& probability) {
	size_t aux = amountVertices * (size_t) amountVertices;
	std::vector<bool> vector(aux, false);

	for (unsigned int u = 0; u < amountVertices; ++u)
		for (unsigned int v = 0; v < amountVertices; ++v)
			if (u != v)
				if (isToInsert(probability)) {
					aux = u * (size_t) amountVertices + v;
					if (v < u && !vector[aux]) {
						aux = v * (size_t) amountVertices + u;
						vector[aux] = true;
					} else
						vector[aux] = true;
				}

	std::list<Edge>* edges = new std::list<Edge>();

	for (unsigned int u = 0; u < amountVertices; ++u)
		for (unsigned int v = 0; v < amountVertices; ++v) {
			aux = u * (size_t) amountVertices + v;
			if (vector[aux])
				edges->emplace_back(u + 1, v + 1, randomValue(minValue, maxValue));
		}

	vector.clear();
	return edges;
}

std::list<Edge>* completeRegular(const unsigned int& amountVertices, const bool& isDigraph, const int& maxValue, const int& minValue) {
	std::list<Edge>* edges = new std::list<Edge>();

	for (unsigned int u = 1; u <= amountVertices; ++u)
		for (unsigned int v = u + 1; v <= amountVertices; ++v) {
			edges->emplace_back(u, v, randomValue(minValue, maxValue));
			if (isDigraph)
				edges->emplace_back(v, u, randomValue(minValue, maxValue));
		}
	return edges;
}

std::list<Edge>* completeBipartite(const unsigned int& amountVerticesA, const unsigned int& amountVerticesB, const int& maxValue, const int& minValue) {
	const unsigned int amountVertices = amountVerticesA + amountVerticesB;
	std::list<Edge>* edges = new std::list<Edge>();

	for (unsigned int u = 1; u <= amountVerticesB; ++u)
		for (unsigned int v = 1; v <= amountVerticesA; ++v)
			edges->emplace_back(u, v + amountVerticesB, randomValue(minValue, maxValue));

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
		unsigned int amountVertices = 0;
		int minValue = 0;
		int maxValue = minValue;
		bool isValued = false;
		bool isDigraph = isValued;

		if (value == 4) {
			std::cout << "Dgt a qtd de Vertices do conjunto 1." << std::endl;
			unsigned int amountVerticesA;
			std::cin >> amountVerticesA;

			std::cout << "Dgt a qtd de Vertices do conjunto 2." << std::endl;
			unsigned int amountVerticesB;
			std::cin >> amountVerticesB;

			std::cout << "Dgt 0: Grafo nao valorado.\nDgt qualquer valor : Grafo valorado." << std::endl;
			std::cin >> isValued;
			if (isValued) {
				std::cout << "Dgt o maior valor do Grafo valorado." << std::endl;
				std::cin >> maxValue;
				std::cout << "Dgt o menor valor do Grafo valorado." << std::endl;
				std::cin >> minValue;
			}

			isDigraph = false;

			edges = completeBipartite(amountVerticesA, amountVerticesB, maxValue, minValue);

			amountVertices = amountVerticesA + amountVerticesB;
		} else {
			std::cout << "Dgt a qtd de Vertices." << std::endl;
			std::cin >> amountVertices;

			std::cout << "Dgt 0: Grafo nao valorado.\nDgt qualquer valor : Grafo valorado." << std::endl;
			std::cin >> isValued;
			if (isValued) {
				std::cout << "Dgt o maior valor do Grafo valorado." << std::endl;
				std::cin >> maxValue;
				std::cout << "Dgt o menor valor do Grafo valorado." << std::endl;
				std::cin >> minValue;
			}

			std::cout << "Dgt 0: Grafo nao dirigido.\nDgt qualquer valor: Grafo dirigido." << std::endl;
			std::cin >> isDigraph;

			if (value == 1 || value == 2) {
				std::cout << "Dgt a probabilidade de insersao" << std::endl;
				float probability;
				std::cin >> probability;

				if (value == 1)
					edges = randow(amountVertices, maxValue, minValue, probability);
				else
					edges = simpleRandow(amountVertices, isDigraph, maxValue, minValue, probability);

			} else if (value == 3)
				edges = completeRegular(amountVertices, isDigraph, maxValue, minValue);
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