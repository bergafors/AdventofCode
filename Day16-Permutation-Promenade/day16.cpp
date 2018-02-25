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
std::string solve_part_two(std::ifstream&);
void dance_round(std::list<char>&, std::ifstream&);

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

	dance_round(programs, file);

	std::string finished(programs.begin(), programs.end());
	return finished;
}

std::string solve_part_two(std::ifstream& file)
{
	const std::size_t list_len = 16;
	std::list<char> programs;
	for (int i = 0; i < list_len; ++i)
		programs.emplace_back('a' + i);

	bool found_cycle = false;
	unsigned long cycle_length;
	unsigned long imax = 1000000000;
	for (unsigned long i = 0; i < imax; ++i) {
		dance_round(programs, file);
		file.clear();
		file.seekg(0, std::ifstream::beg);

		// When (if) we find a cycle to the start we jump forward as many cycle lengths as possible
		// One or more cycles are guaranteed to exist (as the number of possible list permutations is finite),
		// although not one connecting to the start. 
		// Looking for any cycle might the code faster on average, but it would entail searching over a
		// large set of permutations. Luckily there was a cycle to the start in the input data provided for
		// my account. YMMW.
		if (!found_cycle) {
			auto it = programs.begin();
			for (int j = 0; j < list_len && *it == 'a' + j; ++j)
				++it;
			if (it == programs.end()) {
				cycle_length = i + 1;
				i = imax - 1 - imax % cycle_length;
				found_cycle = true;
			}
		}
	}

	std::string finished(programs.begin(), programs.end());
	return finished;
}

void dance_round(std::list<char>& programs, std::ifstream& file)
{
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
}