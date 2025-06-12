#pragma once

#include <cassert>
#include <iostream>
#include <vector>

namespace Graph {

template <typename T>
class Graph {
public:
	Graph() = default;

	Graph(int numVertices, T nullSymbol) 
		: nullSymbol() 
	{
		assert(numVertices >= 0 && "Graph::Graph cannot have negative vertices");

		// Allocate Adjacency Matrix
		adjMatrix = std::vector<std::vector<T>>(
			numVertices, 
			std::vector<T>(numVertices, 0)
		);
	}

	void setEdge(int i, int j, T val) {
		assert(i >= 0 && i < adjMatrix.size() && "Invalid edge used in Graph::setEdge.");
		assert(j >= 0 && j < adjMatrix.size() && "Invalid edge used in setEdge.");

		adjMatrix[i][j] = val;
		adjMatrix[j][i] = val;
	}

	T getEdge(int i, int j) const {
		assert(i >= 0 && i < adjMatrix.size() && "Invalid edge used in Graph::getEdge.");
		assert(j >= 0 && j < adjMatrix.size() && "Invalid edge used in Graph::getEdge.");

		return adjMatrix[i][j];
	}

	bool hasEdge(int i, int j) const {
		assert(i >= 0 && i < adjMatrix.size() && "Invalid edge used in Graph::hasEdge.");
		assert(j >= 0 && j < adjMatrix.size() && "Invalid edge used in Graph::hasEdge.");

		return adjMatrix[i][j] != nullSymbol;
	}

	int numVertices() const {
		return adjMatrix.size();
	}

	void print() const {
		for (int i = 0; i < adjMatrix.size(); i++) {
			for (int j = 0; j < adjMatrix.size(); j++) {
				std::cout << adjMatrix[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}

private:
	std::vector<std::vector<T>> adjMatrix;
	T nullSymbol;
};

};
