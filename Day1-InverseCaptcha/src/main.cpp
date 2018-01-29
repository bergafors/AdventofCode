#include <iostream>
#include <fstream>
#include <string>

int solve_part_one(std::string fname);
int solve_part_two(std::string fname);

int main()
{
	std::string fname;
	std::cout << "This program solves both parts of the Day 1 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input data file path: ";
	std::cin >> fname;

	int digit_sum = solve_part_one(fname);
	if (digit_sum == -1)
		return 0;
	std::cout << "The answer to part one is: " << digit_sum << '\n';

	digit_sum = solve_part_two(fname);
	std::cout << "The answer to part two is: " << digit_sum << '\n';
}

int solve_part_one(std::string fname)
{
	std::ifstream ifs(fname);
	if (!ifs) {
		std::cout << "Invalid file input.\n";
		return -1;
	}

	int digit_sum = 0;
	char first_digit = 0, prev_digit = 0, next_digit = 0;
	char base = '0';

	ifs.get(first_digit);
	prev_digit = first_digit;

	while (ifs.get(next_digit)) {
		if (next_digit == prev_digit)
			digit_sum += prev_digit - base;
		prev_digit = next_digit;
	}

	if (first_digit == prev_digit)
		digit_sum += prev_digit - base;

	return digit_sum;
}

int solve_part_two(std::string fname)
{
	int digit_sum = 0;

	return digit_sum;
}