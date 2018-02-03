/*
This is a solution to both parts of the Day 6 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/6.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <set>

int solve_part_one(std::ifstream&);
int solve_part_two(std::ifstream&);

int main()
{
	std::string fname;
	std::cout << "This program solves both parts of the Day 6 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input data file path: ";
	std::cin >> fname;

	if (std::ifstream ifs(fname); ifs) {
		std::cout << "The answer to part one is: " << solve_part_one(ifs) << '\n';
	}
	else {
		std::cout << "Couldn't open file.\n";
		return 0;
	}

	if (std::ifstream ifs(fname); ifs) {
		std::cout << "The answer to part two is: " << solve_part_two(ifs) << '\n';
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

	auto vint_comp = [](const auto& a, const auto&b) {
		for (auto ita = a.begin(), itb = b.begin(); ita != a.end() && itb != b.end();) {
			if (*ita < *itb)
				return true;
			if (*ita > *itb)
				return false;
			++ita;
			++itb;
		}
		return false;
	};
	// Using a set provides O(logn) detection of duplicates (not counting the comp function).
	// An unordered set would be even better (O(1)) but writing a hash function is too much work.
	std::set<std::vector<int>, decltype(vint_comp)> prev_banks(vint_comp);

	int niter = 0;
	auto vsize = mem_banks.size();
	while (prev_banks.insert(mem_banks).second) {
		++niter;
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
	}

	return niter;
}
int solve_part_two(std::ifstream& ifs)
{
	return 0;
}