/*
This is a solution to both parts of the Day 2 puzzle of Advent of Code 2017.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
			if (num > line_max) {
				line_max = num;
			}
			else if (num < line_min) {
				line_min = num;
			}
		}
		check_sum += line_max - line_min;
	}

	return check_sum;
}

int solve_part_two(std::ifstream& ifs) {
	return 0;
}