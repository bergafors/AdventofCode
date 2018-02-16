/*
This is a solution to both parts of the Day 12 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/12.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <ios>
#include <algorithm>

int solve_part_one(std::ifstream&);
int solve_part_two(std::ifstream&);

int main()
{

	std::string fname;
	std::cout << "This program solves both parts of the Day 12 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input data file path: ";
	std::cin >> fname;

	if (std::ifstream ifs(fname); ifs) {
		std::cout << "The answer to part one is: " << solve_part_one(ifs) << '\n';
	}
	else {
		std::cout << "Couldn't open file.\n";
	}

	if (std::ifstream ifs(fname); ifs) {
		std::cout << "The answer to part two is: " << solve_part_two(ifs) << '\n';
	}
	else {
		std::cout << "Couldn't open file.\n";
	}

	return 0;
}

int solve_part_one(std::ifstream& ifs)
{
	std::set<std::set<int>> groups;
	for (std::string line; std::getline(ifs, line);) {
		std::vector<int> sub_group;
		std::istringstream linestream(line);

		int leading_digit = 0;
		linestream >> leading_digit;
		sub_group.push_back(leading_digit);

		linestream.seekg(4, std::ios::cur);
		for (std::string digit; std::getline(linestream, digit, ',');)
			sub_group.push_back(std::stoi(digit));

		// Find which, if any, existing groups this new sub group is connected to.
		std::vector<decltype(groups)::iterator> connections;
		std::for_each(sub_group.begin(), sub_group.end(), [&](auto digit) {
			for (auto it = groups.begin(); it != groups.end(); ++it) {
				if (auto itg = it->find(digit); itg != it->end())
					connections.push_back(it);
			}
		});


		// Merge newly connected groups
		if (connections.empty()) {
			groups.insert(std::set<int>(sub_group.begin(), sub_group.end()));
		} else {
			auto merge_target = connections.back();
			merge_target->insert(sub_group.begin(), sub_group.end());
			for (auto it : connections) {
				merge_target->insert(it->begin(), it->end());
				groups.erase(it);
			}
		}

	}

	return 0;
}

int solve_part_two(std::ifstream&)
{
	return 0;
}