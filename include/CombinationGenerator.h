#pragma once

#include <cassert>
#include <numeric>
#include <vector>
#include <algorithm>

template <typename T>
class CombinationGenerator {
public:
	CombinationGenerator(const std::vector<T>& set, size_t comboSize)
		: set(set), comboSize(comboSize), hasReachedEnd(false)
	{
		assert(comboSize <= set.size() && 
	 		"PermutationSize cannot be larger than the size of the base set."
	 	);

		// Create bitmask with leading 1's for first combo
		bitmask.resize(set.size());
		for (size_t i = 0; i < comboSize; i++) {
			bitmask[i] = true;
		}
	}

	CombinationGenerator(size_t numElements, size_t comboSize)
		: comboSize(comboSize), hasReachedEnd(false)
	{
		assert(comboSize <= numElements && 
	 		"PermutationSize cannot be larger than the size of the base set."
	 	);

		// Create a set from indices 0..N-1
		set.resize(numElements);
		std::iota(set.begin(), set.end(), 0);

		// Create bitmask with leading 1's for first combo
		bitmask.resize(set.size());
		for (size_t i = 0; i < comboSize; i++) {
			bitmask[i] = true;
		}
	}

	// Algorithm found on: https://rosettacode.org/wiki/Combinations#C.2B.2B
	bool next(std::vector<T>& out) {
		if (hasReachedEnd) return false;

		// Gather current combination
		out.clear();
		for (size_t i = 0; i < bitmask.size(); i++) {
			if (bitmask[i]) {
				out.emplace_back(set[i]);
			}
		}

		// Permute to next combination if possible
		if (!std::prev_permutation(bitmask.begin(), bitmask.end())) {
			hasReachedEnd = true;
		}
		return true;
	}

private:
	std::vector<T> set;
	std::vector<bool> bitmask;
	size_t comboSize;
	bool hasReachedEnd;
};
