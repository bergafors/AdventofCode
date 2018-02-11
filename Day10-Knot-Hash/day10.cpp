/*
This is a solution to both parts of the Day 9 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/9.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <numeric>
#include <vector>
#include <algorithm>
#include <iomanip>

int solve_part_one(std::ifstream&);
std::string solve_part_two(std::ifstream&);
std::vector<int>::iterator tie_knot(std::vector<int>::iterator, int, std::vector<int>&);

int main()
{

	std::string fname;
	std::cout << "This program solves both parts of the Day 9 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input data file path: ";
	std::cin >> fname;

	if (std::ifstream ifs(fname); ifs) {
		std::cout << "The answer to part one is: " << solve_part_one(ifs) << '\n';
	}
	else {
		std::cout << "Couldn't open file.\n";
	}

	if (std::ifstream ifs(fname); ifs) {
		std::cout << "The answer to part two is: " << solve_part_two(ifs) << '\n';
	}
	else {
		std::cout << "Couldn't open file.\n";
	}

	return 0;
}

int solve_part_one(std::ifstream& ifs)
{
	std::vector<int> num_list(256);
	std::iota(num_list.begin(), num_list.end(), 0);

	std::string line;
	std::getline(ifs, line);
	std::istringstream linestream(line);

	auto pos = num_list.begin();
	const auto b = num_list.begin();
	const auto e = num_list.end();
	std::string slength;
	for (int skip = 0; std::getline(linestream, slength, ','); ++skip) {
		const auto length = std::stoi(slength);

		auto end_of_knot = tie_knot(pos, length, num_list);

		// Advance position
		const auto nskip_to_end = e - end_of_knot;
		pos = nskip_to_end > skip ? end_of_knot + skip : b + (skip - nskip_to_end); 
		
	}

	return num_list[0]*num_list[1];
}

std::string solve_part_two(std::ifstream& ifs)
{
	std::vector<int> num_list(256);
	std::iota(num_list.begin(), num_list.end(), 0);
	
	std::vector<int> vlengths;
	for (char c = 0; ifs.get(c) && c != '\n';)
		vlengths.push_back(c);
	vlengths.insert(vlengths.end(), { 17, 31, 73, 47, 23 });

	auto pos = num_list.begin();
	int skip = 0;
	for (int i = 0; i < 64; ++i) {
		const auto b = num_list.begin();
		const auto e = num_list.end();
		for (const auto& length : vlengths) {
			auto end_of_knot = tie_knot(pos, length, num_list);

			// Advance position
			const auto nskip_to_end = e - end_of_knot;
			pos = nskip_to_end > skip ? end_of_knot + skip : b + (skip - nskip_to_end) % 256;

			++skip;
		}
	}

	// Compute the dense version of the hash knot
	std::vector<int> dense_hash(16);
	for (int i = 0; i < 16 ; ++i) {
		auto start = num_list.begin() + 16 * i;
		auto pos = start;
		dense_hash[i] = *start;
		
		while (pos - start < 15) {
			++pos;
			dense_hash[i] ^= *pos;
		}
	}

	// Rewrite hash in hex
	std::ostringstream oss;
	oss << std::hex;
	for (const auto& i : dense_hash)
		oss << i;
	
	return oss.str();
		
}

std::vector<int>::iterator tie_knot(std::vector<int>::iterator pos, int length, std::vector<int>& num_list)
{
	const auto b = num_list.begin();
	const auto e = num_list.end();

	const auto nstep = length - 1;

	auto first = pos;
	const auto nstep_to_end = e - first;

	auto last = nstep_to_end > nstep ? first + nstep : b + (nstep - nstep_to_end);
	const auto end_of_knot = last + 1 == e ? b : last + 1;

	for (int i = 0; i < length / 2; ++i) {
		std::swap(*first, *last);
		first = first + 1 == e ? b : first + 1;
		last = last == b ? e - 1 : last - 1;
	}

	return end_of_knot;
}