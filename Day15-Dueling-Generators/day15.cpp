/*
This is a solution to both parts of the Day 15 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/15.
*/

#include <iostream>
#include <bitset>

unsigned long long solve_part_one(int, int);
unsigned long long solve_part_two(int, int);
unsigned long long& get_next_multiple(
	unsigned long long& curr,
	const unsigned long long& multiplier,
	const unsigned long long& factor,
	const unsigned long long& divisor);

int main()
{

	int a_start = 0, b_start = 0;
	std::cout << "This program solves both parts of the Day 15 puzzle of Advent of Code 2017\n";
	std::cout << "Input the values generator A and B start with, respectively: ";
	std::cin >> a_start >> b_start;

	std::cout << "A: " << a_start << " B: " << b_start << '\n';
	
	std::cout << "The answer to part one is: " << solve_part_one(a_start, b_start) << '\n';
	std::cout << "The answer to part two is: " << solve_part_two(a_start, b_start) << '\n';

	return 0;
}

unsigned long long solve_part_one(int a_start, int b_start)
{
	using range_type = unsigned long long;
	using value_type = unsigned long long;

	const range_type niter = 40000000;
	const value_type divisor = 2147483647;
	const value_type a_factor = 16807, b_factor = 48271;

	value_type a_val = a_start, b_val = b_start;

	range_type nmatch = 0;
	for (range_type i = 0; i < niter; ++i) {
		std::bitset<16> a_bits(a_val), b_bits(b_val);
		if (a_bits == b_bits)
			++nmatch;
		a_val = ( a_factor * a_val ) % divisor;
		b_val = ( b_factor * b_val ) % divisor;
	}

	return nmatch;
}

unsigned long long solve_part_two(int a_start, int b_start)
{
	using range_type = unsigned long long;
	using value_type = unsigned long long;

	const range_type niter = 5000000;
	const value_type divisor = 2147483647;
	const value_type a_factor = 16807, b_factor = 48271;
	const value_type a_multiplier = 4, b_multiplier = 8;

	value_type a_val = a_start, b_val = b_start;

	if (a_val % a_multiplier != 0)
		a_val = get_next_multiple(a_val, a_multiplier, a_factor, divisor);
	if (b_val % b_multiplier != 0)
		b_val = get_next_multiple(b_val, b_multiplier, b_factor, divisor);

	range_type nmatch = 0;
	for (range_type i = 0; i < niter; ++i) {
		std::bitset<16> a_bits(a_val), b_bits(b_val);
		if (a_bits == b_bits)
			++nmatch;

		a_val = get_next_multiple(a_val, a_multiplier, a_factor, divisor);
		b_val = get_next_multiple(b_val, b_multiplier, b_factor, divisor);
	}

	return nmatch;
}

unsigned long long& get_next_multiple(
	unsigned long long& curr, 
	const unsigned long long& multiplier,
	const unsigned long long& factor,
	const unsigned long long& divisor)
{
	do {
		curr = (curr * factor) % divisor;
	} while (curr % multiplier != 0);

	return curr;
}