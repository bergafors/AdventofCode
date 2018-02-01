#include <iostream>
#include <cmath>
#include <algorithm>
int solve_part_one(int);

int main()
{
	int input = 0;
	std::cout << "This program solves both parts of the Day 3 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the puzzle input: ";
	std::cin >> input;

	std::cout << "The answer to part one is: " << solve_part_one(input) << '\n';

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