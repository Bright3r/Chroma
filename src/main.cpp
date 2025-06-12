#include <iostream>
#include <cstdint>
#include "Graph.h"

typedef uint8_t Color;

int main() {
	Graph::Graph<Color> graph(5);

	std::cout << "Hello World!" << std::endl;
	return 0;
}
