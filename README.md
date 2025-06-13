## Build Instructions
- mkdir build

# Manual Build
- cmake --preset=release
- cmake --build build

# Build Script
- chmod +x cmake-build
- ./cmake-build release

## Usage
./build/Chroma filepath-to-graph k_1 k_2 ... k_n
Example: ./build/Chroma graphs/30-4-6 4 6

./build/Chroma clique-size-int k_1 k_2 ... k_n
Example: ./build/Chroma 30 4 6
