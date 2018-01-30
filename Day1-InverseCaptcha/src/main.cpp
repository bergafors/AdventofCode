/*
This is a solution to both parts of the Day 1 puzzle of Advent of Code 2017.

Both parts consider a sequence of digits, where the sequence is taken to loop 
around to the beginning at the end.

The first problem is to find the sum of all digits that are followed 
by the same digit. E.g, 112211 gives 1+2+1+1.

The second problem is to find the sum of all digits where the same digit appears
at an forward offset of half the sequence length. E.g, 1212 gives 1+2.
*/

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
	std::ifstream ifs(fname);

	ifs.ignore(std::numeric_limits<std::streamsize>::max());
	auto half_fsize = ifs.gcount()/2;
	ifs.clear();
	ifs.seekg(0, std::ios_base::beg);

	char base = '0';
	int digit_sum = 0;
	for (std::streamsize i = 0; i < half_fsize; ++i) {
		auto curr_digit = ifs.peek();
		ifs.seekg(half_fsize, std::ios_base::cur);
		auto offset_digit = ifs.peek();
		
		// curr_pos + half_fsize = offset_pos gives offset_pos + half_fsize = curr_pos (the
		// sequence is a loop). So we count the contribution twice if the digits match, and
		// offset it by only looping through half the stream.
		if (curr_digit == offset_digit)
			digit_sum += 2*(curr_digit - base);

		ifs.seekg(i + 1, std::ios_base::beg);
	}

	return digit_sum;
}