/*
This is a solution to both parts of the Day 6 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/6.

Define SET to use std::set instead of std::unordered_set for detecting duplicates in part one.
std::unorderd_set is about 3-4 times faster.

*/


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <chrono>

int solve_part_one(std::ifstream&);
int solve_part_two(std::ifstream&);

int main()
{
	
	std::string fname;
	std::cout << "This program solves both parts of the Day 6 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input data file path: ";
	std::cin >> fname;

	if (std::ifstream ifs(fname); ifs) {
		auto begin = std::chrono::high_resolution_clock::now();
		std::cout << "The answer to part one is: " << solve_part_one(ifs) << '\n';
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Elapsed time: "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms.\n";
	}
	else {
		std::cout << "Couldn't open file.\n";
		return 0;
	}

	if (std::ifstream ifs(fname); ifs) {
		auto begin = std::chrono::high_resolution_clock::now();
		std::cout << "The answer to part two is: " << solve_part_two(ifs) << '\n';
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Elapsed time: " 
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms.\n";
	}
	else {
		std::cout << "Couldn't open file.\n";
		return 0;
	}

	return 0;
}

int solve_part_one(std::ifstream& ifs)
{
	std::vector<int> mem_banks(std::istream_iterator<int>(ifs), std::istream_iterator<int>{});

// Using std::unordered_set is about 3-4 times faster than using std::set here, taking
// O(1) time to search as compared to O(logn).
#ifdef SET
	auto vint_comp = [](const auto& a, const auto& b) {
		for (auto ita = a.begin(), itb = b.begin(); ita != a.end() && itb != b.end();) {
			if (*ita < *itb)
				return true;
			if (*ita > *itb)
				return false;
			++ita;
			++itb;
		}
		return false;
	};.
	std::set<std::vector<int>, decltype(vint_comp)> prev_banks(vint_comp);
#else
	auto ivec_hash = [](const auto& iv) {
		std::size_t seed = iv.size();
		for (auto& i : iv) {
			seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	};
	std::unordered_set<std::vector<int>, decltype(ivec_hash)> prev_banks(0, ivec_hash);
#endif

	int niter = 0;
	while (prev_banks.insert(mem_banks).second) {
		auto it = std::max_element(mem_banks.begin(), mem_banks.end());
		auto nblocks = *it;
		*it = 0;

		while (nblocks > 0) {
			++it;
			if (it == mem_banks.end())
				it = mem_banks.begin();
			*it += 1;
			--nblocks;
		}

		++niter;
	}
	return niter;
}

int solve_part_two(std::ifstream& ifs)
{
	std::vector<int> mem_banks(std::istream_iterator<int>(ifs), std::istream_iterator<int>{});

	auto ivec_hash = [](const auto& iv) {
		std::size_t seed = iv.size();
		for (auto& i : iv) {
			seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	};

	auto ivec_equal = [](const auto& a, const auto& b) {
		for (auto ita = a.begin(), itb = b.begin(); ita != a.end() && itb != b.end();) {
			if (*ita != *itb)
				return false;
			++ita;
			++itb;
		}
		return true;
	};

	std::unordered_map<std::vector<int>, int, decltype(ivec_hash), 
		decltype(ivec_equal)> prev_banks(0, ivec_hash, ivec_equal);

	int niter = 0;
	auto val = prev_banks.insert({ mem_banks, niter });
	while (val.second){
		auto it = std::max_element(mem_banks.begin(), mem_banks.end());
		auto nblocks = *it;
		*it = 0;

		while (nblocks > 0) {
			++it;
			if (it == mem_banks.end())
				it = mem_banks.begin();
			*it += 1;
			--nblocks;
		}

		++niter;
		val = prev_banks.insert({ mem_banks, niter });
	}

	return niter-val.first->second;
}