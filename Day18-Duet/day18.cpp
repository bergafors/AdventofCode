/*
This is a solution to both parts of the Day 18 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/18.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <array>

int solve_part_one(std::ifstream&);
int solve_part_two(std::ifstream&);

int main()
{

	std::string fname;;
	std::cout << "This program solves both parts of the Day 18 puzzle of Advent of Code 2017\n";
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



int solve_part_one(std::ifstream& file)
{
	using namespace std::string_literals;

	long long last_sound = 0;
	std::unordered_map<std::string, long long> registers;
	std::vector<std::ifstream::pos_type> stream_pos{file.tellg()};
	for (std::string line; std::getline(file, line);) {
		stream_pos.push_back(file.tellg());
		std::istringstream iss(line);

		std::cout << registers["a"] << ' ';
		std::cout << line << std::endl;

		std::string op_code, sX, sY;
		iss >> op_code >> sX >> sY;

		long long Y = 0;
		if (sY.size() > 0) {
			if (sY[0] >= 'a')
				Y = registers[sY];
			else
				Y = std::stoi(sY);
		}

		if (op_code == "set"s)
			registers[sX] = Y;
		else if (op_code == "add"s)
			registers[sX] += Y;
		else if (op_code == "mul"s)
			registers[sX] *= Y;
		else if (op_code == "mod"s)
			registers[sX] %= Y;
		else if (op_code == "rcv"s) {
			if (registers[sX] != 0)
				return last_sound;
		}
		else if (op_code == "snd"s)
			last_sound = registers[sX];
		else if (op_code == "jgz"s) {
			long long val = (sX[0] >= 'a') ? registers[sX] : std::stoi(sX);
			if (val <= 0)
				continue;
			if (file.eof() && (Y <= -1 || Y >= 2))
				file.clear();
			if (Y < 0) {
				for (int i = 0; i < std::abs(Y) + 1; ++i)
					stream_pos.pop_back();
				file.seekg(stream_pos.back());
			}
			else {
				std::string temp;
				for (int i = 0; i < Y - 1; ++i) {
					std::getline(file, temp);
					stream_pos.push_back(file.tellg());
				}
			}		
		}
		else {
			std::cout << "Error in part one." <<  op_code <<'\n';
			return 0;
		}


	}

	return -1;
}

int solve_part_two(std::ifstream&)
{
	return 0;
}