/*
This is a solution to both parts of the Day 3 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/4.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
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
	int nvalid = 0;	
	for (std::string line; std::getline(ifs, line);) {
		std::istringstream iss(line);
		std::vector<std::string> words;

		{
			iss.ignore(std::numeric_limits<std::streamsize>::max());
			auto iss_size = iss.gcount();
			iss.seekg(0, std::ios::beg);
			words.reserve(static_cast<std::size_t>(iss_size));
		}
		
		for (std::string word; iss >> word;)
			words.push_back(word);

		std::sort(words.begin(), words.end());

		bool is_valid = true;
		for (auto it = words.cbegin(); it != words.cend() - 1; ++it) {
			if (*it == *(it + 1)) {
				is_valid = false;
				break;
			}
		}

		if (is_valid)
			++nvalid;
	}

	return nvalid;
}

int solve_part_two(std::ifstream& ifs)
{
	int nvalid = 0;
	for (std::string line; std::getline(ifs, line);) {
		std::istringstream iss(line);
		std::vector<std::string> words;

		{
			iss.ignore(std::numeric_limits<std::streamsize>::max());
			auto iss_size = iss.gcount();
			iss.seekg(0, std::ios::beg);
			words.reserve(static_cast<std::size_t>(iss_size));
		}

		for (std::string word; iss >> word;) {
			std::sort(word.begin(), word.end());
			words.push_back(word);
		}

		std::sort(words.begin(), words.end());

		bool is_valid = true;
		for (auto it = words.cbegin(); it != words.cend() - 1; ++it) {
			if (*it == *(it + 1)) {
				is_valid = false;
				break;
			}
		}

		if (is_valid)
			++nvalid;
	}

	return nvalid;
}
