/*
This is a solution to both parts of the Day 13 puzzle of Advent of Code 2017.
The problem description can be found at https://adventofcode.com/2017/day/13.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::pair<int, int> solve_both_parts(std::ifstream&);

int main()
{

	std::string fname;
	std::cout << "This program solves both parts of the Day 13 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input data file path: ";
	std::cin >> fname;

	std::ifstream file;

	try {

		file.exceptions(std::ifstream::failbit);
		file.open(fname);
	}
	catch (const std::ifstream::failure&) {
		std::cout << "Error reading file\n";
	}

	try {
		file.exceptions(std::ifstream::badbit);

		auto ans = solve_both_parts(file);
		std::cout << "The answer to part one is: " << ans.first << '\n';
		std::cout << "The answer to part two is: " << ans.second << '\n';
	}
	catch (const std::ifstream::failure&) {
		std::cout << "Error reading file\n";
	}

	return 0;
}

std::pair<int, int> solve_both_parts(std::ifstream& file)
{
	std::vector<std::pair<int, int>> layers;

	for (std::string line; std::getline(file, line);) {
		std::istringstream linestream(line);

		int depth = 0, range = 0;
		linestream >> depth;
		linestream.ignore(line.length(), ' ');
		linestream >> range;

		layers.emplace_back(std::move(depth), std::move(range));
	}

	int severity = 0;
	for (const auto& l : layers) {
		const auto& arrival_time = l.first;
		const auto cycle_length = 2 * l.second - 2;

		if (arrival_time % cycle_length == 0)
			severity += l.first*l.second;
	}

	auto passes = [&](int k) {
		for (const auto& l : layers) {
			const auto& arrival_time = l.first + k;
			const auto cycle_length = 2 * l.second - 2;

			if (arrival_time % cycle_length == 0)
				return false;
		}
		return true;
	};

	int t = 0;
	while (!passes(t)) ++t;

	return { severity, t};
}