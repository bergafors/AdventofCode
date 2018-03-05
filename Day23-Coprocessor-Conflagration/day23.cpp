/*
This is a solution to both parts of the Day 23 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/23.
*/

#include <iostream>
#include <fstream>
#include <sstream>

#include <unordered_map>
#include <string>
#include <string_view>
#include <vector>

using val_type = long long;

val_type solve_part_one(std::vector<std::string>);
val_type solve_part_two();

// Evaluates instruction jnz with %X and %Y as arguments. jnz makes a jump 
// to a new line in the instruction set. %cur_line is updated accordingly.
// Returns false is jnz tries to jump outside of [0, ENDLINE]. true otherwise.
bool eval_jnz(val_type& X, const val_type& Y, val_type& cur_line, const val_type& ENDLINE);

std::vector<std::string> parse_input(std::ifstream& input_file);

int main()
{
	std::string fname;;
	std::cout << "This program solves both parts of the Day 23 puzzle of Advent of Code 2017\n";
	std::cout << "Enter the input data input_file path: ";
	std::cin >> fname;

	std::ifstream input_file;
	try {
		input_file.exceptions(std::ifstream::failbit);
		input_file.open(fname);
	}
	catch (std::ifstream::failure&) {
		std::cout << "Error opening input_file.\n";
	}

	std::vector<std::string> instructions;
	try {
		input_file.exceptions(std::ifstream::badbit);
		instructions = parse_input(input_file);
	}
	catch (std::ifstream::failure&) {
		std::cout << "Error reading input_file.\n";
	}

	std::cout << "The answer to part one is: " << solve_part_one(instructions) << '\n';
	std::cout << "The answer to part two is: " << solve_part_two() << '\n';

	return 0;
}

std::vector<std::string> parse_input(std::ifstream& input_file)
{
	std::vector<std::string> instructions;
	for (std::string line; std::getline(input_file, line);)
		instructions.push_back(line);
	return instructions;
}

val_type solve_part_one(std::vector<std::string> instructions)
{
	std::unordered_map<std::string, val_type> registers;

	val_type nmul = 0;
	val_type ENDLINE = static_cast<val_type>(instructions.size() - 1);
	for (val_type cur_line = 0; cur_line <= ENDLINE - 1;) {
		std::istringstream iss(instructions[static_cast<std::size_t>(cur_line)]);

		std::string op_code, sX, sY;
		iss >> op_code >> sX >> sY;

		val_type x = 0, y = 0;
		val_type& X = (sX[0] >= 'a') ? registers[sX] : (x = std::stoi(sX));
		val_type& Y = (sY[0] >= 'a') ? registers[sY] : (y = std::stoi(sY));

		if (op_code == "set")
			X = Y;
		else if (op_code == "sub")
			X -= Y;
		else if (op_code == "mul") {
			X *= Y;
			++nmul;
		}
		else if (op_code == "jnz") {
			bool success = eval_jnz(X, Y, cur_line, ENDLINE);
			if (!success)
				return nmul;
			continue;
		}
		else {
			std::cout << "Error in part one. " << op_code << '\n';
			throw;
		}

		++cur_line;
	}

	return nmul;
}

val_type solve_part_two() 
{
	val_type b = 81 * 100 + 100'000;
	val_type c = b + 17'000;
	val_type h = 0;
	for (; b < c + 17; b+=17) {
		bool flag = false;
		for (val_type d = 2; d < b; ++d) {
			if (b % d == 0)
				flag = true;
		}
		if (flag)
			++h;
	}

	return h;
}

bool eval_jnz(val_type& X, const val_type& Y, val_type& cur_line, const val_type& ENDLINE)
{
	if (X == 0 || Y == 0) {
		++cur_line;
		return true;
	}

	if (Y < 0) {
		if (cur_line < std::abs(Y))
			return false;
		cur_line += Y;
	}
	else if (Y > 0) {
		if (cur_line + Y > ENDLINE)
			return false;
		cur_line += Y;
	}

	return true;
}