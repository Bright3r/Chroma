#pragma once

#include <cassert>
#include <filesystem>
#include <iostream>
#include <cstdint>
#include <unordered_map>
#include "Graph.h"

namespace Chroma {

typedef int32_t Color;
typedef std::unordered_map<Color, int> RamseyMap;

struct ChromaticityCount {
	int totalCliqueCount = 0;
	int monochromaticCount = 0;
	int nonmonochromaticCount = 0;
	std::unordered_map<Color, int> colorCounts;

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

	int size() const;
	Color getEdge(int i, int j) const;
	void setEdge(int i, int j, Color color);
	bool hasEdge(int i, int j) const;
	bool loadGraph(std::filesystem::path filename);
	ChromaticityCount countGraph(const RamseyMap& colors);

private:
	Graph::Graph<Color> graph;
	bool hasLoadedGraph;
};

};	// End of namespace Chroma
