/*
This is a solution to both parts of the Day 2 puzzle of Advent of Code 2017.

The input is a file cointaining rows of numbers.

The first problem is to find the difference between the largest and smallest
number on each row, and then calculate the sum of the differences.

The second problem is to find the single pair of numbers that is evenly divisble on
each row, and then calculate the sum of each such fraction.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

int solve_part_one(std::ifstream&);
int solve_part_two(std::ifstream&);

int main()
{
	std::string fname;
	std::cout << "This program solves both parts of the Day 2 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input data file path: ";
	std::cin >> fname;

	{
		std::ifstream ifs(fname);
		if (!ifs) {
			std::cout << "Couldn't open file.\n";
			return 0;
		}
		std::cout << "The answer to part one is: " << solve_part_one(ifs) << '\n';
	}

	{
		std::ifstream ifs(fname);
		if (!ifs) {
			std::cout << "Couldn't open file.\n";
			return 0;
		}
		std::cout << "The answer to part two is: " << solve_part_two(ifs) << '\n';
	}

	return 0;
}

int solve_part_one(std::ifstream& ifs)
{
	int check_sum = 0;
	std::string line;
	while (std::getline(ifs, line)) {
		std::istringstream iss(line);

		int line_max = 0, line_min = 0;
		iss >> line_max;
		iss >> line_min;
		if (line_max < line_min) {
			int temp = line_max;
			line_max = line_min;
			line_min = temp;
		}

		int num = 0;
		while (iss >> num) {
			if (num > line_max)
				line_max = num;
			else if (num < line_min)
				line_min = num;
		}
		check_sum += line_max - line_min;
	}

	return check_sum;
}

int solve_part_two(std::ifstream& ifs) {
	int fraction_sum = 0;
	std::string line;
	while (std::getline(ifs, line)) {
		std::istringstream iss(line);
		std::vector<int> line_nums;

		{
			iss.ignore(std::numeric_limits<std::streamsize>::max());
			auto iss_size = iss.gcount();
			iss.seekg(0, std::ios::beg);
			line_nums.reserve(static_cast<std::size_t>(iss_size));
		}

		{
			int num = 0;
			while (iss >> num) {
				line_nums.push_back(num);
			}
		}

		bool found_pair = false; // As we know there is only one divisble pair per line we can
                                 // exit the loop when it's found.
		for (auto it = line_nums.cbegin(); it != line_nums.cend()-1; ++it) {
			for (auto itt = it + 1; itt != line_nums.cend(); ++itt) {
				if (*it % *itt == 0 || *itt % *it == 0) {
					found_pair = true;
					fraction_sum += std::max(*it / *itt, *itt / *it);
					break;
				}
			}
			if (found_pair)
				break;
		}
	}

	return fraction_sum;
}