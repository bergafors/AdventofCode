/*
This is a solution to both parts of the Day 9 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/9.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::pair<int, int> solve_both_parts(std::ifstream&);
int main()
{

	std::string fname;
	std::cout << "This program solves both parts of the Day 9 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input data file path: ";
	std::cin >> fname;

	if (std::ifstream ifs(fname); ifs) {
		auto p = solve_both_parts(ifs);
		std::cout << "The answer to part one is: " << p.first << '\n';
		std::cout << "The answer to part two is: " << p.second << '\n';
	}
	else {
		std::cout << "Couldn't open file.\n";
		return 0;
	}
	return 0;
}

std::pair<int, int> solve_both_parts(std::ifstream& ifs)
{
	int score = 0, depth = 0, num_trash = 0;
	bool trash = false, ignore_next = false;
	for (char ch = 0; ifs >> ch;) {
		if (ignore_next) {
			ignore_next = false;
			continue;
		}
		else if (trash) {
			switch (ch)
			{
			case '>':
				trash = false;
				break;
			case '!':
				ignore_next = true;
				break;
			default:
				++num_trash;
				break;
			}
			continue;
		}

		switch (ch)
		{
		case '{':
			depth += 1;
			score += depth;
			break;
		case '}':
			depth -= 1;
			break;
		case '<':
			trash = true;
			break;
		default:
			break;
		}
	}

	return {score, num_trash};
}