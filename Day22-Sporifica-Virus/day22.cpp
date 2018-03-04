/*
This is a solution to both parts of the Day 22 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/22.
*/

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <deque>

#include <limits>

enum class Turn { LEFT, RIGHT, BACK};

using node_grid = std::deque<std::deque<char>>;
using direction = std::pair<int, int>;
using position = std::pair<long int, long int>;

std::size_t solve_part_one(node_grid ng);
std::size_t solve_part_two(node_grid ng);

// Read node_grid from %input_file
node_grid parse_input(std::ifstream& input_file);

// Turn direction %dir to %turn.
direction turn_direction(const direction& dir, Turn turn);

// Move %pos in direction %dir. Enlarge %ng if necessary.
position move_forward(const direction& dir, const position& pos, node_grid& ng);

int main()
{
	std::string fname;;
	std::cout << "This program solves both parts of the Day 22 puzzle of Advent of Code 2017\n";
	std::cout << "Enter the input data file path: ";
	std::cin >> fname;

	std::ifstream file;
	try {
		file.exceptions(std::ifstream::failbit);
		file.open(fname);
	}
	catch (std::ifstream::failure&) {
		std::cout << "Error opening file.\n";
	}

	node_grid ng;
	try {
		file.exceptions(std::ifstream::badbit);
		ng = parse_input(file);
	}
	catch (std::ifstream::failure&) {
		std::cout << "Error reading file.\n";
	}

	std::cout << "The answer to part one is: " << solve_part_one(ng) << '\n';
	std::cout << "The answer to part two is: " << solve_part_two(ng)<< '\n';

	return 0;
}

node_grid parse_input(std::ifstream& input_file)
{
	node_grid ng;
	for (std::string line; std::getline(input_file, line);)
		ng.emplace_back(line.begin(), line.end());

	return ng;
}

std::size_t solve_part_one(node_grid ng)
{
	const int NITER = 10000;

	position pos{ ng.size() / 2 , ng.front().size() / 2 };
	direction dir{ -1, 0 };

	std::size_t ninfected = 0;
	for (int i = 0; i < NITER; ++i) {
		const auto& [row, col] = pos;
		if (ng[row][col] == '.') {
			ng[row][col] = '#';
			++ninfected;
			dir = turn_direction(dir, Turn::LEFT);
		}
		else {
			ng[row][col] = '.';
			dir = turn_direction(dir, Turn::RIGHT);
		}

		pos = move_forward(dir, pos, ng);
	}

	return ninfected;
}

std::size_t solve_part_two(node_grid ng)
{
	const long int NITER = 10000000;

	position pos{ ng.size() / 2 , ng.front().size() / 2 };
	direction dir{ -1, 0 };

	std::size_t ninfected = 0;
	for (long int i = 0; i < NITER; ++i) {
		const auto&[row, col] = pos;
		if (ng[row][col] == '.') {
			ng[row][col] = 'W';
			dir = turn_direction(dir, Turn::LEFT);
		}
		else if (ng[row][col] == 'W') {
			ng[row][col] = '#';
			++ninfected;
		}
		else if (ng[row][col] == '#') {
			ng[row][col] = 'F';
			dir = turn_direction(dir, Turn::RIGHT);
		}
		else /*if (ng[row][col] == 'F')*/ {
			ng[row][col] = '.';
			dir = turn_direction(dir, Turn::BACK);
		}

		pos = move_forward(dir, pos, ng);

		if (i % 1000000 == 0)
			std::cout << "Part two is " << (int) ((double) i / NITER * 100) << "% done.\n";
	}

	return ninfected;
}

position move_forward(const direction& dir, const position& pos, node_grid& ng)
{
	position new_pos{pos.first + dir.first, pos.second + dir.second};

	std::size_t nrows_unsigned = ng.size();
	std::size_t ncols_unsigned = ng.front().size();

	if (nrows_unsigned == std::numeric_limits<long int>::max()
		|| ncols_unsigned == std::numeric_limits<long int>::max()) {
		std::cout << "Exceeded allowed grid size.\n";
		throw;
	}

	int nrows = static_cast<int>(ng.size());
	int ncols = static_cast<int>(ng.front().size());

	if (new_pos.first < 0) {
		ng.emplace_front(std::deque<char>(ncols, '.'));
		new_pos.first = 0;
		return new_pos;
	}
	else if (new_pos.first > nrows - 1) {
		ng.emplace_back(std::deque<char>(ncols, '.'));
		return new_pos;
	}

	if (new_pos.second < 0) {
		for (auto& row : ng)
			row.emplace_front('.');
		new_pos.second = 0;
		return new_pos;
	}
	else if (new_pos.second > ncols - 1) {
		for (auto& row : ng)
			row.emplace_back('.');
		return new_pos;
	}

	return new_pos;
}

direction turn_direction(const direction& dir, Turn turn)
{
	static direction up{ -1, 0 }, down{ 1, 0 }, left{ 0, -1 }, right{ 0, 1 };

	if (turn == Turn::LEFT) {
		if (dir == up)
			return left;
		else if (dir == left)
			return down;
		else if (dir == down)
			return right;
		else
			return up;
	}
	else if (turn == Turn::RIGHT) {
		if (dir == up)
			return right;
		else if (dir == right)
			return down;
		else if (dir == down)
			return left;
		else
			return up;
	}
	else /*if (turn == Turn::BACK)*/{
		if (dir == up)
			return down;
		else if (dir == right)
			return left;
		else if (dir == down)
			return up;
		else
			return right;
	}
}