/*
This is a solution to both parts of the Day 3 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/3.
*/

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <utility>

int solve_part_one(int);
int solve_part_two(int);

std::pair<int, int> next_square(int, int, std::vector<std::vector<int>>&);
std::vector<std::pair<int, int>> neighbours(int, int);

int main()
{
	int input = 0;
	std::cout << "This program solves both parts of the Day 3 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the puzzle input: ";
	std::cin >> input;

	std::cout << "The answer to part one is: " << solve_part_one(input) << '\n';
	std::cout << "The answer to part two is: " << solve_part_two(input) << '\n';

	return 0;
}

int solve_part_one(int input)
{	
	int root = 0;
	while ((root+1) * (root+1) <= input) {
		++root;
	}

	if (root * root == input) {
		return root - 1; 
	}

	int side_len = root + 1;
	if (side_len % 2 == 0)
		++side_len;

	const int x_cord = (side_len - 1) / 2;	

	const int corner_dist = (side_len * side_len - input ) % (side_len-1);
	const int corner_dist_min = std::min(corner_dist, std::abs(side_len - 1 - corner_dist));
	const int y_cord = (side_len - 1) / 2  - corner_dist_min;

	return x_cord + y_cord;
}

int solve_part_two(int input)
{
	std::size_t grid_size = 13;
	std::vector<std::vector<int>> grid(grid_size, std::vector<int>(grid_size, 0));

	auto x = grid_size / 2, y = grid_size / 2;
	grid[x][y] = 1;
	grid[++x][y] = 1;

	do {
		auto xy = next_square(x, y, grid);
		x = xy.first;
		y = xy.second;
		for (auto [p, q] : neighbours(x, y))
			grid[x][y] += grid[p][q];
	} while (grid[x][y] <= input);

	return grid[x][y];
}

std::pair<int, int> next_square(int x, int y, std::vector<std::vector<int>>& grid)
{	
	int del_x = x - grid.size() / 2, del_y = y - grid.size() / 2;
	int dist = std::max(std::abs(del_x), std::abs(del_y));

	if (del_y == -dist) {
		if (del_x > -dist)
			return { x - 1, y };
		return { x, y + 1 };
	}
	else if (del_x == -dist) {
		if (del_y < dist)
			return { x, y + 1 };
		return { x + 1, y };
	}
	else if (del_y == dist) {
		if (del_x <= dist)
			return { x + 1, y };
	}
	if (del_y > -dist)
		return { x, y - 1 };
	return { x - 1, y };
}

std::vector<std::pair<int, int>> neighbours(int x, int y)
{
	std::vector<std::pair<int, int>> ngbs;
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if (j == 0 && i == 0)
				continue;
			ngbs.push_back({ x + i, y + j });
		}
	}
	return ngbs;
}