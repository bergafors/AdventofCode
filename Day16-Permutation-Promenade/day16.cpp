/*
This is a solution to both parts of the Day 16 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/16.
*/

#include <iostream>

int solve_part_one(int, int);
int solve_part_two(int, int);

int main()
{

	int a_start = 0, b_start = 0;
	std::cout << "This program solves both parts of the Day 16 puzzle of Advent of Code 2017\n";
	std::cout << "Input the values generator A and B start with, respectively: ";
	std::cin >> a_start >> b_start;

	std::cout << "The answer to part one is: " << solve_part_one(a_start, b_start) << '\n';
	std::cout << "The answer to part two is: " << solve_part_two(a_start, b_start) << '\n';

	return 0;
}