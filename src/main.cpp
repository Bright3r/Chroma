#include <filesystem>
#include <iostream>
#include <unordered_map>
#include "Chroma.h"

int main() {
	// Load Graph
	Chroma::ChromaticGraph graph;
	bool success = graph.loadGraph("graphs/6.adj");
	if (!success) {
		std::cerr << "Failed to load graph." << std::endl;
		return -1;
	}
	std::cout << "Loaded Graph!" << std::endl;

	// Setup colors to check R(3, 3)
	std::unordered_map<Chroma::Color, size_t> colors;
	colors[1] = 3;
	colors[2] = 3;

	// Find and print chromaticity counts
	Chroma::ChromaticityCount counts = graph.countGraph(colors);
	counts.print();

	return 0;
}
