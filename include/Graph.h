#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

namespace Graph {

template <typename T>
class Graph {
public:
	Graph() = default;

	Graph(int numVertices, T nullSymbol) 
		: numVerts(numVertices), nullSymbol(nullSymbol) 
	{
		assert(numVertices >= 0 && "Graph::Graph cannot have negative vertices");

		// Allocate Adjacency Matrix (flattened)
		adjMatrix = std::vector<T>(numVertices * (numVertices + 1) / 2, nullSymbol);
	}

	int index(int i, int j) const noexcept {
		if (i > j) std::swap(i, j);
		return i * numVerts - (i * (i + 1)) / 2 + j;
	}

	void setEdge(int i, int j, T val) noexcept {
		assert(i >= 0 && i < numVerts && "Invalid edge used in Graph::setEdge.");
		assert(j >= 0 && j < numVerts && "Invalid edge used in setEdge.");

		adjMatrix[index(i, j)] = val;
	}

	T getEdge(int i, int j) const noexcept {
		assert(i >= 0 && i < numVerts && "Invalid edge used in Graph::getEdge.");
		assert(j >= 0 && j < numVerts && "Invalid edge used in Graph::getEdge.");

		return adjMatrix[index(i, j)];
	}

	bool hasEdge(int i, int j) const noexcept {
		assert(i >= 0 && i < numVerts && "Invalid edge used in Graph::hasEdge.");
		assert(j >= 0 && j < numVerts && "Invalid edge used in Graph::hasEdge.");

		return adjMatrix[index(i, j)] != nullSymbol;
	}

	int numVertices() const noexcept {
		return numVerts;
	}

	void setNullSymbol(T newNullSymbol) {
		nullSymbol = newNullSymbol;
	}

	T getNullSymbol() const noexcept {
		return nullSymbol;
	}

	void print() const {
		for (int i = 0; i < numVerts; i++) {
			for (int j = 0; j < numVerts; j++) {
				std::cout << adjMatrix[index(i, j)] << " ";
			}
			std::cout << std::endl;
		}
	}

private:
	std::vector<T> adjMatrix;
	int numVerts;
	T nullSymbol;
};

};
