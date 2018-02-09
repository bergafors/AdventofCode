/*
This is a solution to both parts of the Day 9 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/9.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <numeric>
#include <vector>
#include <algorithm>

std::pair<int, int> solve_both_parts(std::ifstream&);

int main()
{

	std::string fname;
	std::cout << "This program solves both parts of the Day 9 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input data file path: ";
	std::cin >> fname;

	if (std::ifstream ifs(fname); ifs) {
		auto p = solve_both_parts(ifs);
		std::cout << "The answer to part one is: " << p.first << '\n';
		std::cout << "The answer to part two is: " << p.second << '\n';
	}
	else {
		std::cout << "Couldn't open file.\n";
	}

	return 0;
}

std::pair<int, int> solve_both_parts(std::ifstream& ifs)
{
	std::vector<int> num_list(255);
	std::iota(num_list.begin(), num_list.end(), 0);

	auto pos = num_list.begin();
	for (int skip = 0, length = 0; ifs >> length; ++skip) {
		auto first = pos;
		auto last = pos;

		if (auto dist = num_list.end() - 1 - first; dist >= length) {
			last += length;
			std::reverse(first, last);
		}
		else {
			last = num_list.begin() + length - dist;
			while (first - last > 0)
		}
		pos += skip;
	}

	return {0, 0};
}