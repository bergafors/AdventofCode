/*
This is a solution to both parts of the Day 14 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/14.

This solution reuses code from Day 10 to calculate knot hashes on the input data.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>
#include <bitset>

int solve_part_one(const std::string&);
std::string knot_hash(std::string);
std::vector<int>::iterator tie_knot(std::vector<int>::iterator pos, int length, std::vector<int>& num_list);

int main()
{

	std::string key_string;
	std::cout << "This program solves both parts of the Day 13 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input key string: ";
	std::cin >> key_string;

	std::cout << "The answer to part one is: " << solve_part_one(key_string) << '\n';

	return 0;
}

int solve_part_one(const std::string& key)
{
	const std::size_t nrows = 128;

	int nfree = 0;
	for (std::size_t i = 0; i < nrows; ++i) {
		auto row_input = key + "-" + std::to_string(i);
		auto row_hash = knot_hash(row_input);

		for (std::size_t j = 0; j < row_hash.length(); ++j) {
			std::string sval = row_hash.substr(j, 1);
			int n = std::stoi(sval, nullptr, 16);
			nfree += std::bitset<4>(n).count();
		}
	}

	return nfree;
}


std::string knot_hash(std::string input)
{
	std::vector<int> num_list(256);
	std::iota(num_list.begin(), num_list.end(), 0);

	std::vector<int> vlengths;
	std::istringstream iss(input);
	for (char c = 0; iss.get(c) && c != '\n';)
		vlengths.push_back(c);
	vlengths.insert(vlengths.end(), { 17, 31, 73, 47, 23 });

	auto pos = num_list.begin();
	int skip = 0;
	for (int i = 0; i < 64; ++i) {
		const auto b = num_list.begin();
		const auto e = num_list.end();
		for (const auto& length : vlengths) {
			const auto end_of_knot = tie_knot(pos, length, num_list);

			// Skip to next position
			const auto nskip_to_end = e - end_of_knot;
			pos = nskip_to_end > skip ? end_of_knot + skip : b + (skip - nskip_to_end) % 256;

			++skip;
		}
	}

	// Compute the dense version of the hash knot
	std::vector<int> dense_hash(16);
	for (int i = 0; i < 16; ++i) {
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