/*
This is a solution to both parts of the Day 16 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/16.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>

std::string solve_part_one(std::ifstream&);
int solve_part_two(std::ifstream&);

int main()
{

	std::string fname;;
	std::cout << "This program solves both parts of the Day 16 puzzle of Advent of Code 2017\n";
	std::cout << "Enter the input data file path: ";
	std::cin >> fname;

	std::ifstream file;
	try {
		file.exceptions(std::ifstream::failbit);
		file.open(fname);

	}
	catch (const std::ifstream::failure&) {
		std::cout << "Error opening file\n";
		return 0;
	}

	try {
		file.exceptions(std::ifstream::badbit);

		std::cout << "The answer to part one is: " << solve_part_one(file) << '\n';

		file.clear();
		file.seekg(0, std::ifstream::beg);
		std::cout << "The answer to part two is: " << solve_part_two(file) << '\n';
	}
	catch (std::ifstream::failure&) {
		std::cout << "Error reading file\n";
		return 0;
	}

	return 0;
}

std::string solve_part_one(std::ifstream& file)
{
	std::list<char> programs;
	for (int i = 0; i < 16; ++i)
		programs.emplace_back('a' + i);

	int nspin = 0;
	for (std::string input; std::getline(file, input, ',');) {
		std::istringstream linestream(input);

		char c = 0;
		linestream.get(c);
		switch (c)
		{
		case 's':
		{
			int n = 0;
			linestream >> n;
			auto it = programs.end();
			for (int i = 0; i < n; ++i)
				--it;
			programs.splice(programs.begin(), programs, it, programs.end());
			break;
		}
		case 'x': 
			{
				int a = 0, b = 0;
				std::string num;

				std::getline(linestream, num, '/');
				a = std::stoi(num);
				std::getline(linestream, num, '/');
				b = std::stoi(num);

				auto ita = programs.begin();
				auto itb = programs.begin();
				for (int i = 0; i < a; ++i)
					++ita;
				for (int i = 0; i < b; ++i)
					++itb;

				std::swap(*ita, *itb);

			}
			break;
		case 'p':
			{
				char a = 0, b = 0;
				linestream.get(a);
				linestream.get(b).get(b);

				auto ita = std::find(programs.begin(), programs.end(), a);
				auto itb = std::find(programs.begin(), programs.end(), b);
				std::swap(*ita, *itb);
			}
			break;
		default:
			std::cout << "ERROR\n";
			break;
		}
	}

	std::string finished(programs.begin(), programs.end());
	return finished;
}
int solve_part_two(std::ifstream& file)
{

	return 0;
}