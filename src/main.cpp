#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <unordered_map>
#include "Graph.h"
#include "CombinationGenerator.h"

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

bool loadGraph(std::filesystem::path filename, Graph::Graph<Color>& graph) {
	// Open File
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Could not open file: " << filename << std::endl;
		return false;
	}

	// Parse File
	std::string line;
	std::string word;

	// Read graph size
	std::getline(file, line);
	std::stringstream ss(line);
	ss >> word;
	size_t numVertices = std::stoi(word);

	// Read graph's null edge symbol
	ss = std::stringstream(line);
	ss >> word;
	Color nullSymbol = std::stoi(word);

	// Read File line by line to construct graph
	graph = Graph::Graph<Color>(numVertices, nullSymbol);
	size_t i = 0;
	while (std::getline(file, line)) {
		// Read line for this vertex's edge colors
		ss = std::stringstream(line);
		size_t j = 0;
		while (ss >> word) {
			Color color = std::stoi(word);
			graph.setEdge(i, j, color);
			j++;
		}
		i++;
	}

	file.close();
	return true;
}

ChromaticityCount countGraph(Graph::Graph<Color>& graph, 
			     std::unordered_map<Color, size_t> colors) 
{
	ChromaticityCount counts;
	for (const auto& [color, subcliqueSize] : colors) {
		// Initialize color count to 0
		counts.colorCounts[color] = 0;

		// Count the monochromatic subcliques of the current color
		CombinationGenerator<Color> generator(graph.numVertices(), subcliqueSize);
		std::vector<Color> subclique(subcliqueSize);
		while (generator.next(subclique)) {
			// Check if current combination of vertices forms a monochromatic
			bool isMonochromatic = true;
			bool isValidClique = true;
			for (size_t i = 0; i < subcliqueSize; i++) {
				for (size_t j = i + 1; j < subcliqueSize; j++) {
					size_t v0 = subclique[i];
					size_t v1 = subclique[j];
					if (!graph.hasEdge(v0, v1)) {
						isValidClique = false;
						break;
					}
					else if (graph.getEdge(v0, v1) != color) {
						isMonochromatic = false;
					}
				}
			}

			// Add to graph chromaticity statistics
			if (isValidClique) {
				counts.totalCliqueCount++;
				if (isMonochromatic) {
					counts.monochromaticCount++;
					counts.colorCounts[color]++;
				}
				else counts.nonmonochromaticCount++;
			}
		}
	}

	return counts;
}

int main() {
	Graph::Graph<Color> graph;
	bool success = loadGraph("graphs/6.adj", graph);
	if (!success) {
		std::cerr << "Failed to load graph." << std::endl;
		return -1;
	}
	std::cout << "Loaded Graph!" << std::endl;

	std::unordered_map<Color, size_t> colors;
	colors[1] = 3;
	colors[2] = 3;
	ChromaticityCount counts = countGraph(graph, colors);
	counts.print();

	return 0;
}
