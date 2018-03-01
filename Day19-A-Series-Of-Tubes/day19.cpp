
/*
This is a solution to both parts of the Day 19 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/19.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <vector>
#include <algorithm>
#include <iterator>

std::string solve_part_one(std::vector<std::vector<char>>&);
std::string solve_part_two(std::vector<std::vector<char>>&);

std::vector<std::vector<char>> read_input(std::ifstream&);
bool step_forward(std::vector<std::vector<char>>&,
	std::pair<std::size_t, std::size_t>&, std::pair<std::size_t, std::size_t>&);

int main()
{

	std::string fname;;
	std::cout << "This program solves both parts of the Day 19 puzzle of Advent of Code 2017\n";
	std::cout << "Enter the input data file path: ";
	std::cin >> fname;

	std::ifstream file;
	try {
		file.exceptions(std::ifstream::failbit);
		file.open(fname);

	}
	catch (const std::ifstream::failure&) {
		std::cout << "Error opening file\n";
		return 0;
	}

	std::vector<std::vector<char>> routing_diagram;
	try {
		file.exceptions(std::ifstream::badbit);
		routing_diagram = read_input(file);
	}
	catch (std::ifstream::failure&) {
		std::cout << "Error reading file\n";
		return 0;
	}

	std::cout << "The answer to part one is: " << solve_part_one(routing_diagram) << '\n';
	std::cout << "The answer to part two is: " << solve_part_two(routing_diagram) << '\n';

	return 0;
}

std::vector<std::vector<char>> read_input(std::ifstream& file)
{
	std::vector<std::vector<char>> routing_diagram;
	for (std::string line; std::getline(file, line);) 
		routing_diagram.push_back(std::vector<char>(line.begin(), line.end()));

	return routing_diagram;
}

std::string solve_part_one(std::vector<std::vector<char>>& routing_diagram)
{
	std::pair<std::size_t, std::size_t> pos;
	pos.first = 0;
	const auto start = std::find(routing_diagram.front().begin(), routing_diagram.front().end(), '|');
	pos.second = std::distance(routing_diagram.front().begin(), start);

	decltype(pos) dir{ 1, 0 };

	std::string seen_letters;
	do {
		const auto& symbol = routing_diagram[pos.first][pos.second];
		if ('A' <= symbol && symbol <= 'Z')
			seen_letters.push_back(symbol);
	} while (step_forward(routing_diagram, pos, dir));

	return seen_letters;
}

// Return true if a step forward was successfully taken
bool step_forward(std::vector<std::vector<char>>& routing_diagram,
	std::pair<std::size_t, std::size_t>& pos, std::pair<std::size_t, std::size_t>& dir)
{
	const auto nrow = routing_diagram.size();
	const auto ncol = routing_diagram.front().size();

	char c = routing_diagram[pos.first][pos.second];
	if (c == '+') { // Then change step direction
		// Can we walk here && Not walking back && Is there a path here
		if (pos.first + 1 < nrow && dir.first != -1
			&& routing_diagram[pos.first + 1][pos.second] != ' ')
			dir = { 1, 0 };
		else if (pos.first - 1 >= 0 && dir.first != 1
			&& routing_diagram[pos.first - 1][pos.second] != ' ')
			dir = { -1, 0 };
		else if (pos.second + 1 < ncol && dir.second != -1
			&& routing_diagram[pos.first][pos.second + 1] != ' ')
			dir = { 0, 1 };
		else if (pos.second - 1 >= 0 && dir.second != 1
			&& routing_diagram[pos.first][pos.second - 1] != ' ')
			dir = { 0, -1 };
		else {
			std::cout << "Error? Couldn't find new step direction.\n";
			return false;
		}

	}

	pos = { pos.first + dir.first, pos.second + dir.second };
	// Check if this is the end of the path
	if (routing_diagram[pos.first][pos.second] == ' ')
		return false;
	else if (0 > pos.first || nrow <= pos.first || 0 > pos.second || ncol <= pos.second)
		return false;

	return true;
}


std::string solve_part_two(std::vector<std::vector<char>>& routing_diagram)
{
	return "Part two";
}