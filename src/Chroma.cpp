#include "Chroma.h"
#include "CombinationGenerator.h"
#include <cstdlib>
#include <fstream>
#include <unordered_set>

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

ChromaticGraph::ChromaticGraph(int numVertices, std::vector<Color> colorList, Color nullColor) 
	: hasLoadedGraph(false)
{
	graph = Graph::Graph<Color>(numVertices, nullColor);
	for (int i = 0; i < numVertices; i++) {
		for (int j = i + 1; j < numVertices; j++) {
			if (i == j) {
				graph.setEdge(i, j, nullColor);
			}
			else {
				graph.setEdge(i, j, colorList[rand() % colorList.size()]);
			}
		}
	}
	hasLoadedGraph = true;
}

int ChromaticGraph::size() const noexcept {
	return graph.numVertices();
}

Color ChromaticGraph::getEdge(int i, int j) const noexcept {
	return graph.getEdge(i, j);
}

void ChromaticGraph::setEdge(int i, int j, Color color) noexcept {
	graph.setEdge(i, j, color);
}

bool ChromaticGraph::hasEdge(int i, int j) const noexcept {
	return graph.hasEdge(i, j);
}

void ChromaticGraph::print() const {
	graph.print();
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
	int numVertices = std::stoi(word);

	// Read graph's null edge symbol
	ss >> word;
	Color nullSymbol = std::stoi(word);

	// Read File line by line to construct graph
	graph = Graph::Graph<Color>(numVertices, nullSymbol);
	int i = 0;
	while (std::getline(file, line)) {
		// Read line for this vertex's edge colors
		ss = std::stringstream(line);
		int j = 0;
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

ChromaticityCount ChromaticGraph::countGraph(const RamseyMap& colors) {
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
			for (int i = 0; i < subcliqueSize; i++) {
				for (int j = i + 1; j < subcliqueSize; j++) {
					int v0 = subclique[i];
					int v1 = subclique[j];
					Color edge = graph.getEdge(v0, v1);
					if (edge == graph.getNullSymbol()) {
						isValidClique = false;
						break;
					}
					else if (edge != color) {
						isMonochromatic = false;
						break;
					}
				}

				// if (!isValidClique || !isMonochromatic) break;
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
};

std::vector<Color> ChromaticGraph::getColorList() const {
	std::unordered_set<Color> colorSet;
	for (int i = 0; i < graph.numVertices(); i++) {
		for (int j = i + 1; j < graph.numVertices(); j++) {
			if (graph.hasEdge(i, j)) {
				colorSet.insert(graph.getEdge(i, j));
			}
		}
	}

	// Return set of colors as a vector
	std::vector<Color> colorList;
	colorList.insert(colorList.end(), colorSet.begin(), colorSet.end());
	return colorList;
}

};	// End of namespace Chroma
