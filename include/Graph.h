#pragma once

#include <cstdint>
#include <vector>

namespace Graph {

template <typename T>
class Graph {
public:
	Graph(size_t numVertices) {
		// Allocate Adjacency Matrix
		adjMatrix = std::vector<std::vector<T>>(
			numVertices, 
			std::vector<T>(numVertices, 0)
		);
	}

	void setEdge(uint32_t i, uint32_t j, T val) {
		adjMatrix[i][j] = val;
		adjMatrix[j][i] = val;
	}

	T getEdge(uint32_t i, uint32_t j) {
		return adjMatrix[i][j];
	}

	size_t numVertices() const {
		return adjMatrix.size();
	}

private:
	std::vector<std::vector<T>> adjMatrix;
};

};
