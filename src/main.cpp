#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <random>
#include <unordered_map>
#include "Chroma.h"

#define MAX_ITERATIONS 1000000
#define INITIAL_TEMP 5.0f
#define COOLING_RATE 0.9999f
#define REHEATING_POINT 10000

// RNG
static std::random_device rd;
static std::mt19937 rng(rd());

struct StateChange {
	int i, j;
	Chroma::Color oldColor, newColor;
};

double temperature(int i, int itersSinceLastImprovement) {
	double reheatFactor = itersSinceLastImprovement > REHEATING_POINT ? INITIAL_TEMP * .2f : 0;
	return INITIAL_TEMP * std::pow(COOLING_RATE, i) + reheatFactor;
}

StateChange getNeighbor(const Chroma::ChromaticGraph& graph, const std::vector<Chroma::Color> colorList) {
	std::uniform_int_distribution<int> edgeDist(0, graph.size() - 1);
	std::uniform_int_distribution<int> colorDist(0, colorList.size() - 1);

	// Find valid edge to modify
	int i = edgeDist(rng), j = edgeDist(rng);
	while (!graph.hasEdge(i, j)) {
		i = edgeDist(rng);
		j = edgeDist(rng);
	}

	// Get new color
	Chroma::Color oldColor = graph.getEdge(i, j);
	Chroma::Color newColor = colorList[colorDist(rng)];
	while (newColor == oldColor) {
		newColor = colorList[colorDist(rng)];
	}

	StateChange stateChange = { i, j, oldColor, newColor };
	return stateChange;
}

double eval(Chroma::ChromaticGraph& graph, const std::unordered_map<Chroma::Color, int>& ramseyColors) {
	Chroma::ChromaticityCount counts = graph.countGraph(ramseyColors);
	return static_cast<double>(counts.monochromaticCount);
}

double probability(double delta, double temp) {
	// P = e^(delta_E / T)
	return std::exp(-delta / temp);
}

void moveToState(Chroma::ChromaticGraph& graph, const StateChange& stateChange) {
	graph.setEdge(stateChange.i, stateChange.j, stateChange.newColor);
}

void revertToState(Chroma::ChromaticGraph& graph, const StateChange& stateChange) {
	graph.setEdge(stateChange.i, stateChange.j, stateChange.oldColor);
}

void anneal(Chroma::ChromaticGraph& graph, std::vector<Chroma::Color> colorList, const Chroma::RamseyMap ramseyColors) {
	// Setup for RNG
	std::uniform_real_distribution<double> probabilityDist(0.f, 1.f);

	// Simulated Annealing Loop
	double E_old = eval(graph, ramseyColors);
	int itersSinceLastImprovement = 0;
	for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
		// Update temperature
		double temp = temperature(iter, itersSinceLastImprovement);

		// Generate new state
		StateChange stateChange = getNeighbor(graph, colorList);

		// Calculate energy of new state
		moveToState(graph, stateChange);
		double E_new = eval(graph, ramseyColors);

		// Check if new state is a solution
		if (E_new == 0) {
			break;
		}

		double delta = E_new - E_old;
		double prob = probability(delta, temp);
		// If new state is better, accept it
		if (delta <= 0) {
			E_old = E_new;
			if (delta < 0) itersSinceLastImprovement = 0;
		}
		// If new state is worse, calculate probability of moving to a worse state
		else {
			if (probabilityDist(rng) >= prob) {
				// Reject state change
				revertToState(graph, stateChange);
			}
			else {
				// Accept state change
				E_old = E_new;
			}
			itersSinceLastImprovement++;
		}

		// Display Progress
		if (iter % 100 == 0) {
			std::cout << "Iteration " << iter << ": temp=" << temp 
				<< ", prob=" << prob << ", E=" << E_old << std::endl;
		}
	}
}

bool loadGraph(int argc, char** argv, Chroma::ChromaticGraph& graph, 
	       std::vector<int>& cliqueSizes) {
	// Check number of program arguments
	if (argc < 4) {
		std::cerr << "Insufficient number of arguments." << std::endl;
		std::cout << "Usage: chroma filepath-to-graph k_1 k_2 ... k_n" << std::endl;
		return false;
	}

	// Load program arguments
	std::filesystem::path filename = argv[1];
	cliqueSizes.clear();
	for (int i = 2; i < argc; i++) {
		cliqueSizes.emplace_back(std::stoi(argv[i]));
	}

	// Read in graph
	std::string fileString = filename.string();
	bool isInt = std::all_of(fileString.begin(), fileString.end(), ::isdigit);
	if (isInt) {
		// If passsed an integer instead of a filename,
		// create a random complete graph of the given size instead
		int numVertices = std::stoi(filename);
		std::vector<Chroma::Color> colorList(argc - 2);
		std::iota(colorList.begin(), colorList.end(), 1);
		graph = Chroma::ChromaticGraph(numVertices, colorList, 0);
	}
	else {
		// Otherwise, load in graph from a file
		bool success = graph.loadGraph(filename);
		if (!success) {
			std::cerr << "Failed to load graph." << std::endl;
			return -1;
		}
		std::cout << "Loaded Graph!" << std::endl;
	}

	return true;
}

int main(int argc, char** argv) {
	// Load in graph
	Chroma::ChromaticGraph graph;
	std::vector<int> cliqueSizes;
	if (!loadGraph(argc, argv, graph, cliqueSizes)) {
		std::cerr << "Terminating program." << std::endl;
		return -1;
	}

	// Get list of edge colors used in graph
	std::vector<Chroma::Color> colorList = graph.getColorList();
	if (colorList.size() != cliqueSizes.size()) {
		std::cerr << "Mismatch between number of edge colors and subcliques" << std::endl;
		return -1;
	}

	// Setup "Ramsey Map" from edge color to corresponding clique size
	Chroma::RamseyMap ramseyColors;
	for (int i = 0; i < colorList.size(); i++) {
		ramseyColors[colorList[i]] = cliqueSizes[i];
	}

	// Run Simulated Annealing to minimize monochromatic subcliques
	anneal(graph, colorList, ramseyColors);

	// Find and print chromaticity counts
	Chroma::ChromaticityCount counts = graph.countGraph(ramseyColors);
	counts.print();

	std::cout << "\nEdge Coloring: " << std::endl;
	graph.print();

	return 0;
}

// int main() {
// 	CombinationGenerator<int> gen(5, 3);
// 	std::vector<int> out;
// 	while (gen.next(out)) {
// 		for (int i = 0; i < out.size(); i++) {
// 			std::cout << out[i] << " ";
// 		}
// 		std::cout << std::endl;
// 	}
//
// 	return 0;
// }

// int main(int argc, char** argv) {
// 	// Load in graph
// 	Chroma::ChromaticGraph graph;
// 	std::vector<int> cliqueSizes;
// 	if (!loadGraph(argc, argv, graph, cliqueSizes)) {
// 		std::cerr << "Terminating program." << std::endl;
// 		return -1;
// 	}
//
// 	// Create RamseyMap
// 	Chroma::RamseyMap ramseyColors;
// 	std::vector<Chroma::Color> colorList = graph.getColorList();
// 	for (int i = 0; i < colorList.size(); i++) {
// 		ramseyColors[colorList[i]] = cliqueSizes[i];
// 	}
//
// 	// Time chromaticity check
// 	auto startTime = std::chrono::high_resolution_clock::now();
//
// 	Chroma::ChromaticityCount counts = graph.countGraph(ramseyColors);
// 	counts.print();
//
// 	auto endTime = std::chrono::high_resolution_clock::now();
// 	float deltaTime = std::chrono::duration<float>(endTime - startTime).count();
// 	std::cout << "Time Elapsed: " << deltaTime << std::endl;
//
// 	return 0;
// }
