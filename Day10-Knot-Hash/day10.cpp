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
	std::vector<int> num_list(256);
	std::iota(num_list.begin(), num_list.end(), 0);

	std::string line;
	std::getline(ifs, line);
	std::istringstream linestream(line);

	auto pos = num_list.begin();
	const auto b = num_list.begin();
	const auto e = num_list.end();
	std::string slength;
	for (int skip = 0; std::getline(linestream, slength, ','); ++skip) {
		const auto length = std::stoi(slength);
		const auto nstep = length - 1;

		auto first = pos;
		const auto nstep_to_end = e - first;

		auto last = nstep_to_end > nstep ? first + nstep : b + (nstep - nstep_to_end);
		const auto next_pos_start = last + 1 == e ? b : last + 1;

		for (int i = 0; i < length / 2; ++i) {
			std::swap(*first, *last);
			first = first + 1 == e ? b : first + 1;
			last = last == b ? e - 1 : last - 1;
		}

		{
			const auto nskip_to_end = e - next_pos_start;
			pos = nskip_to_end > skip ? next_pos_start + skip : b + (skip - nskip_to_end);
		} 
		
	}

	return {num_list[0]*num_list[1], 0};
}