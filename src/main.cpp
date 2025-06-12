#include <cmath>
#include <filesystem>
#include <iostream>
#include <random>
#include <unordered_map>
#include "Chroma.h"

#define MAX_ITERATIONS 100000
#define INITIAL_TEMP 10
#define COOLING_RATE 0.9999

// RNG
static std::random_device rd;
static std::mt19937 rng(rd());

struct StateChange {
	int i, j;
	Chroma::Color oldColor, newColor;
};

float temperature(int i) {
	float r = 1 - (i + 1) / static_cast<float>(MAX_ITERATIONS);
	float temp = INITIAL_TEMP * std::pow(COOLING_RATE, i);
	return temp;
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

	StateChange stateChange = { i, j, oldColor, newColor };
	return stateChange;
}

float eval(Chroma::ChromaticGraph& graph, const std::unordered_map<Chroma::Color, int>& ramseyColors) {
	Chroma::ChromaticityCount counts = graph.countGraph(ramseyColors);
	return static_cast<float>(counts.monochromaticCount);
}

float probability(float temp) {
	// P = e^(-13/T)
	return std::exp(-13 / temp);
}

void moveToState(Chroma::ChromaticGraph& graph, const StateChange& stateChange) {
	graph.setEdge(stateChange.i, stateChange.j, stateChange.newColor);
}

void revertToState(Chroma::ChromaticGraph& graph, const StateChange& stateChange) {
	graph.setEdge(stateChange.i, stateChange.j, stateChange.oldColor);
}

void anneal(Chroma::ChromaticGraph& graph, std::vector<Chroma::Color> colorList, const Chroma::RamseyMap ramseyColors) {
	// Setup for RNG
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> edgeDist(0, graph.size() - 1);
	std::uniform_int_distribution<int> colorDist(0, colorList.size() - 1);
	std::uniform_real_distribution<float> probabilityDist(0.f, 1.f);

	// Simulated Annealing Loop
	bool isSolved = false;
	float E_old = eval(graph, ramseyColors);
	for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
		// Update temperature
		float temp = temperature(iter);
		std::cout << "Temp: " << temp << std::endl;

		// Generate new state
		StateChange stateChange = getNeighbor(graph, colorList);

		// Calculate energy of new state
		moveToState(graph, stateChange);
		float E_new = eval(graph, ramseyColors);

		// Check if new state is a solution
		if (E_new == 0) {
			break;
		}

		// If new state is worse, calculate probability of moving to a worse state
		float delta = E_new - E_old;
		if (delta > 0) {
			float prob = probability(temp);
			if (probabilityDist(rng) >= prob) {
				// Reject state change
				revertToState(graph, stateChange);
			}
			else {
				// Accept state change
				E_old = E_new;
			}
		}
	}
}

int main() {
	// Load Graph
	Chroma::ChromaticGraph graph;
	bool success = graph.loadGraph("graphs/5.adj");
	if (!success) {
		std::cerr << "Failed to load graph." << std::endl;
		return -1;
	}
	std::cout << "Loaded Graph!" << std::endl;

	// Setup colors to check R(3, 3)
	std::vector<Chroma::Color> colorList;
	colorList.emplace_back(1);
	colorList.emplace_back(2);

	Chroma::RamseyMap ramseyColors;
	ramseyColors[colorList[0]] = 3;
	ramseyColors[colorList[1]] = 3;

	// Run Simulated Annealing to minimize monochromatic subcliques
	anneal(graph, colorList, ramseyColors);

	// Find and print chromaticity counts
	Chroma::ChromaticityCount counts = graph.countGraph(ramseyColors);
	counts.print();

	return 0;
}
