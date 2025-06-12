#pragma once

#include <cassert>
#include <vector>

namespace Graph {

template <typename T>
class Graph {
public:
	Graph() = default;

	Graph(size_t numVertices, T nullSymbol) 
		: nullSymbol() 
	{
		// Allocate Adjacency Matrix
		adjMatrix = std::vector<std::vector<T>>(
			numVertices, 
			std::vector<T>(numVertices, 0)
		);
	}

	void setEdge(size_t i, size_t j, T val) {
		assert(i < adjMatrix.size() && "Invalid edge used in setEdge.");
		assert(j < adjMatrix.size() && "Invalid edge used in setEdge.");

		adjMatrix[i][j] = val;
		adjMatrix[j][i] = val;
	}

	T getEdge(size_t i, size_t j) {
		assert(i < adjMatrix.size() && "Invalid edge used in getEdge.");
		assert(j < adjMatrix.size() && "Invalid edge used in getEdge.");

		return adjMatrix[i][j];
	}

	bool hasEdge(size_t i, size_t j) {
		assert(i < adjMatrix.size() && "Invalid edge used in getEdge.");
		assert(j < adjMatrix.size() && "Invalid edge used in getEdge.");

		return adjMatrix[i][j] != nullSymbol;
	}

	size_t numVertices() const {
		return adjMatrix.size();
	}

private:
	std::vector<std::vector<T>> adjMatrix;
	T nullSymbol;
};

};
