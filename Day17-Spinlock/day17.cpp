/*
This is a solution to both parts of the Day 17 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/17.
*/

#include <iostream>
#include <list>
#include <iterator>

int solve_part_one(int);
int solve_part_two(int);

int main()
{

	int input;
	std::cout << "This program solves both parts of the Day 17 puzzle of Advent of Code 2017\n";
	std::cout << "Input puzzle input: ";
	std::cin >> input;

	std::cout << "The answer to part one is: " << solve_part_one(input) << '\n';
	std::cout << "The answer to part two is: " << solve_part_two(input) << '\n';

	return 0;
}

int solve_part_one(int input)
{
	const std::size_t nstep = input;
	const std::size_t niter = 2017;
	std::list<int> circular_buffer{ 0 };

	auto it = circular_buffer.begin();
	auto itb = circular_buffer.begin();

	std::size_t pos = 0;
	for (std::size_t i = 1; i <= niter; ++i) {
		const auto sz = circular_buffer.size();
		const auto remainder = nstep % sz;
		const std::size_t new_pos = (pos + remainder) % sz;

		int diff = new_pos - pos;
		std::advance(it, diff);
		
		++it;
		it = circular_buffer.insert(it, i);
		pos = new_pos + 1;
	}
	
	++it;
	return *it;
}
int solve_part_two(int input)
{
	return 0;
}