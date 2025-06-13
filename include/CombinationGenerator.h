#pragma once

#include <cassert>
#include <cstdint>
#include <numeric>
#include <vector>
#include <algorithm>

template <typename T>
class CombinationGenerator {
public:
	CombinationGenerator(const std::vector<T>& set, int comboSize)
		: set(set), comboSize(comboSize), isFirstCombo(true), hasReachedEnd(false)
	{
		assert(comboSize <= set.size() && 
	 		"PermutationSize cannot be larger than the size of the base set."
	 	);

		// Create bitmask with leading 1's for first combo
		bitmask = std::vector<uint8_t>(set.size(), 0);
		for (int i = 0; i < comboSize; i++) {
			bitmask[i] = true;
		}
	}

	CombinationGenerator(int numElements, int comboSize)
		: comboSize(comboSize), isFirstCombo(true), hasReachedEnd(false)
	{
		assert(comboSize <= numElements && 
	 		"PermutationSize cannot be larger than the size of the base set."
	 	);

		// Create a set from indices 0..N-1
		set.resize(numElements);
		std::iota(set.begin(), set.end(), 0);

		// Create bitmask with leading 1's for first combo
		bitmask = std::vector<uint8_t>(set.size(), 0);
		for (int i = 0; i < comboSize; i++) {
			bitmask[i] = 1;
		}
	}

	// Algorithm found on: https://rosettacode.org/wiki/Combinations#C.2B.2B
	bool next(std::vector<T>& out) noexcept {
		if (hasReachedEnd) return false;
		if (isFirstCombo) out.resize(comboSize);

		// Gather current combination
		int idx = 0;
		for (int i = 0; i < bitmask.size(); i++) {
			if (bitmask[i]) {
				out[idx++] = set[i];
			}
		}

		// Permute to next combination if possible
		if (!std::prev_permutation(bitmask.begin(), bitmask.end())) {
			hasReachedEnd = true;
		}

		isFirstCombo = false;
		return true;
	}

private:
	std::vector<T> set;
	std::vector<uint8_t> bitmask;
	int comboSize;
	bool isFirstCombo;
	bool hasReachedEnd;
};
