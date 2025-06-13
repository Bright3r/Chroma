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

		indices.resize(comboSize);
	}

	// // Algorithm found on: https://rosettacode.org/wiki/Combinations#C.2B.2B
	// bool next(std::vector<T>& out) noexcept {
	// 	if (hasReachedEnd) return false;
	// 	if (isFirstCombo) out.resize(comboSize);
	//
	// 	// Gather current combination
	// 	int idx = 0;
	// 	int bitmaskSize = bitmask.size();
	// 	for (int i = 0; i < bitmaskSize; i++) {
	// 		if (bitmask[i]) {
	// 			out[idx++] = set[i];
	// 		}
	// 	}
	//
	// 	// Permute to next combination if possible
	// 	if (!std::prev_permutation(bitmask.begin(), bitmask.end())) {
	// 		hasReachedEnd = true;
	// 	}
	//
	// 	isFirstCombo = false;
	// 	return true;
	// }

	bool next(std::vector<T>& out) noexcept {
		if (hasReachedEnd) return false;

		int n = set.size();
		int k = comboSize;

		if (isFirstCombo) {
			// Initialize indices to {0, 1, ..., k-1}
			for (int i = 0; i < k; ++i) {
				indices[i] = i;
			}

			out.resize(k);
			for (int i = 0; i < k; ++i) {
				out[i] = set[indices[i]];
			}

			isFirstCombo = false;
			return true;
		}

		// Find the rightmost index that can be incremented
		int i = k - 1;
		while (i >= 0 && indices[i] == n - k + i) --i;
		if (i < 0) {
			hasReachedEnd = true;
			return false;
		}

		++indices[i];
		for (int j = i + 1; j < k; ++j) {
			indices[j] = indices[j - 1] + 1;
		}

		for (int j = 0; j < k; ++j) {
			out[j] = set[indices[j]];
		}

		return true;
	}

private:
	std::vector<T> set;
	std::vector<uint8_t> bitmask;
	int comboSize;
	bool isFirstCombo;
	bool hasReachedEnd;

	std::vector<int> indices;
};
