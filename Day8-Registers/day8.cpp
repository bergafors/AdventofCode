
/*
This is a solution to both parts of the Day 8 puzzle of Advent of Code 2017.
The problem description can be found at https://adventofcode.com/2017/day/8.
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <algorithm>

int solve_part_one(std::ifstream&);
bool may_execute(int, std::string, int);
int solve_part_two(std::ifstream&);

int main()
{

	std::string fname;
	std::cout << "This program solves both parts of the Day 8 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input data file path: ";
	std::cin >> fname;

	if (std::ifstream ifs(fname); ifs) {
		std::cout << "The answer to part one is: " << solve_part_one(ifs) << '\n';
	}
	else {
		std::cout << "Couldn't open file.\n";
		return 0;
	}

	if (std::ifstream ifs(fname); ifs) {
		std::cout << "The answer to part two is: " << solve_part_two(ifs) << '\n';
	}
	else {
		std::cout << "Couldn't open file.\n";
		return 0;
	}

	return 0;
}

int solve_part_one(std::ifstream& ifs)
{
	std::unordered_map<std::string, int> registers;
	for (std::string line; std::getline(ifs, line);) {
		std::istringstream iss(line);
		
		std::string reg_name;
		iss >> reg_name;

		std::string reg_op;
		iss >> reg_op;

		int val = 0;
		iss >> val;

		bool execute = true;
		if (std::string temp; iss >> temp) { // Then execution condition is present
			std::string cond_reg_name;
			iss >> cond_reg_name;
			auto cond_reg = registers[cond_reg_name];

			std::string cond;
			iss >> cond;

			int cond_val = 0;
			iss >> cond_val;

			execute = may_execute(cond_reg, cond, cond_val);
		}

		if (execute) {
			if (reg_op == "inc")
				registers[reg_name] += val;
			else if (reg_op == "dec")
				registers[reg_name] -= val;
		}
	}

	auto it = std::max_element(registers.begin(), registers.end(), 
		[](const auto& p1, const auto& p2) {
			return p1.second < p2.second;
		});

	return it->second;
}

bool may_execute(int cond_reg, std::string cond, int cond_val)
{

	if (cond == ">" && cond_reg <= cond_val)
		return false;
	else if (cond == ">=" && cond_reg < cond_val)
		return false;
	else if (cond == "<" && cond_reg >= cond_val)
		return false;
	else if (cond == "<=" && cond_reg > cond_val)
		return false;
	else if (cond == "==" && cond_reg != cond_val)
		return false;
	else if (cond == "!=" && cond_reg == cond_val)
		return false;

	return true;
}
int solve_part_two(std::ifstream& ifs)
{
	return 0;
}