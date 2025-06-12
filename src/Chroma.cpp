#include "Chroma.h"
#include "CombinationGenerator.h"
#include <fstream>

namespace Chroma {

ChromaticGraph::ChromaticGraph() 
	: hasLoadedGraph(false) { }

ChromaticGraph::ChromaticGraph(std::filesystem::path filename)
	: hasLoadedGraph(false)
{
	if (loadGraph(filename)) {
		hasLoadedGraph = true;
	}
}

bool ChromaticGraph::loadGraph(std::filesystem::path filename) {
	hasLoadedGraph = false;

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
	hasLoadedGraph = true;
	return true;
}

ChromaticityCount ChromaticGraph::countGraph(std::unordered_map<Color, size_t> colors) {
	assert(hasLoadedGraph && 
		"Cannot invoke ChromaticGraph::countGraph before loading graph."
	);

	ChromaticityCount counts;
	for (const auto& [color, subcliqueSize] : colors) {
		// Initialize color's count to 0
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

};	// End of namespace Chroma
