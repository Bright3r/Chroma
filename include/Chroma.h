#pragma once

#include <cassert>
#include <filesystem>
#include <iostream>
#include <cstdint>
#include <unordered_map>
#include "Graph.h"

namespace Chroma {

typedef uint32_t Color;

struct ChromaticityCount {
	size_t totalCliqueCount = 0;
	size_t monochromaticCount = 0;
	size_t nonmonochromaticCount = 0;
	std::unordered_map<Color, size_t> colorCounts;

	void print() const {
		std::cout << "Total Cliques: " << totalCliqueCount << std::endl;
		std::cout << "Monochromatic Cliques: " << monochromaticCount << std::endl;
		std::cout << "Non-monochromatic Cliques: " << nonmonochromaticCount << std::endl;

		std::cout << "Colored Clique Counts: " << std::endl;
		for (auto& [color, colorCount] : colorCounts) {
			std::cout << "\t" << std::to_string(color) << ": " 
				<< colorCount << std::endl;
		}
	}
};

class ChromaticGraph {
public:
	ChromaticGraph();
	ChromaticGraph(std::filesystem::path filename);

	bool loadGraph(std::filesystem::path filename);
	ChromaticityCount countGraph(std::unordered_map<Color, size_t> colors);

private:
	Graph::Graph<Color> graph;
	bool hasLoadedGraph;
};

};	// End of namespace Chroma
