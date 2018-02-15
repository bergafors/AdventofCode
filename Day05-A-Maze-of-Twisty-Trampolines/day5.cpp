/*
This is a solution to both parts of the Day 5 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/5.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>

int solve_part_one(std::ifstream&);
int solve_part_two(std::ifstream&);

int main()
{
	std::string fname;
	std::cout << "This program solves both parts of the Day 5 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input data file path: ";
	std::cin >> fname;

	{
		std::ifstream ifs(fname);
		if (!ifs) {
			std::cout << "Couldn't open file.\n";
			return 0;
		}
		std::cout << "The answer to part one is: " << solve_part_one(ifs) << '\n';
	}

	{
		std::ifstream ifs(fname);
		if (!ifs) {
			std::cout << "Couldn't open file.\n";
			return 0;
		}
		std::cout << "The answer to part two is: " << solve_part_two(ifs) << '\n';
	}

	return 0;
}

int solve_part_one(std::ifstream& ifs)
{
	int njumps = 0;
	std::vector<int> offset_list(std::istream_iterator<int>(ifs), std::istream_iterator<int>{});
	for (auto it = offset_list.begin(); it >= offset_list.begin() && it < offset_list.end(); ++njumps) {
		it += (*it)++;
	}
	return njumps;
}
int solve_part_two(std::ifstream& ifs)
{
	int njumps = 0;
	std::vector<int> offset_list(std::istream_iterator<int>(ifs), std::istream_iterator<int>{});
	for (auto it = offset_list.begin(); it >= offset_list.begin() && it < offset_list.end(); ++njumps) {
		auto offset = *it;
		if (offset >= 3)
			--*it;
		else
			++*it;
		it += offset;
	}
	return njumps;
}