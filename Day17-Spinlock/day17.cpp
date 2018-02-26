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

	std::size_t pos = 0;
	auto it = circular_buffer.begin();
	for (std::size_t i = 1; i <= niter; ++i) {
		const auto sz = circular_buffer.size();
		const auto remainder = nstep % sz;
		const std::size_t new_pos = (pos + remainder) % sz;

		int diff = new_pos - pos;
		std::advance(it, diff);
		pos = new_pos;
		
		// Move one step extra to simulate insertion ahead of new_pos.
		++it;
		it = circular_buffer.insert(it, i);
		// Update pos to the position of the inserted value.
		++pos;
	}
	
	++it;
	return *it;
}
int solve_part_two(int input)
{
	const std::size_t nstep = input;
	const long niter = 50000000;

	std::size_t pos = 0;
	long val_after_zero = 0;
	for (std::size_t i = 1; i <= niter; ++i) {
		const long sz = i;
		const long remainder = nstep % sz;
		const long new_pos = (pos + remainder) % sz;
		
		if (new_pos == 0)
			val_after_zero = i;
		pos = new_pos + 1;
	}

	return val_after_zero;
}