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
#include <algorithm>
#include <bitset>
#include <list>

int solve_part_one(const std::string&);
int solve_part_two(const std::string&);
std::string knot_hash(std::string);
std::vector<int>::iterator tie_knot(std::vector<int>::iterator pos, int length, std::vector<int>& num_list);

int main()
{

	std::string key_string;
	std::cout << "This program solves both parts of the Day 14 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input key string: ";
	std::cin >> key_string;

	//std::cout << knot_hash("") << '\n';
	//std::cout << knot_hash("AoC 2017") << '\n';
	//std::cout << knot_hash("1,2,3") << '\n';
	//std::cout << knot_hash("1,2,4") << '\n';


	//std::cout << "The answer to part one is: " << solve_part_one(key_string) << '\n';
	std::cout << "The answer to part two is: " << solve_part_two(key_string) << '\n';
	return 0;
}

int solve_part_one(const std::string& key) // Use string_view
{
	const std::size_t nrows = 128;

	int nfree = 0;
	for (std::size_t i = 0; i < nrows; ++i) {
		auto row_input = key + "-" + std::to_string(i);
		auto row_hash = knot_hash(row_input);

		std::bitset<128> bt;
		for (std::size_t j = 0; j < row_hash.length(); ++j) {
			std::string sval = row_hash.substr(j, 1); // Improve
			int n = std::stoi(sval, nullptr, 16);
			bt |= std::bitset<128>(n) << 128 - 4 * (j+1);
		}

		nfree += bt.count();
	}

	return nfree;
}

int solve_part_two(const std::string& key)
{
	const int N = 128;
	const std::size_t nrows = 128;

	std::size_t nregions = 0;
	std::list<std::bitset<N>> prev_borders;
	for (std::size_t i = 0; i < nrows; ++i) {
		auto row_input = key + "-" + std::to_string(i);
		auto row_hash = knot_hash(row_input);
		
		// Create a bitset representation of the hash string
		std::bitset<N> bt;
		for (std::size_t j = 0; j < row_hash.length(); ++j) {
			std::string sval = row_hash.substr(j, 1);
			int n = std::stoi(sval, nullptr, 16);
			if (n > 15)
				std::cout << "ERROR" << '\n';
			bt |= std::bitset<N>(n) << N-4*(j+1);
		}

		std::list<std::bitset<N>> curr_borders;
		for (std::size_t k = 0; k < bt.size();) {
			// Move to beginning of border
			while (k < bt.size() && !bt.test(k))
				++k;

			if (k >= bt.size())
				break;

			// Insert border
			std::bitset<N> border;
			while (k < bt.size() && bt.test(k)) {
				border.set(k);
				++k;
			}

			curr_borders.push_back(border);
		}

		// Merge current borders if they neighbour the same previous border
		for (const auto& border : prev_borders) {
			auto intersects_border = [&](const auto& bt) {
				return (border & bt).count() > 0;
			};

			const auto& b = curr_borders.begin();
			const auto& e = curr_borders.end();

			auto first = std::find_if(b, e, intersects_border);
			for (auto it = first; it != e;) {
				if (intersects_border(*it) && it != first) {
					*first |= *it;
					it = curr_borders.erase(it);
				}
				else
					++it;
			}
		}

		// If a current border doesnt border a previous border it is a new region.
		// If a current border borders more than one previous border, it connects them
		// into a a new region, reducing the number of regions.
		for (const auto& border : curr_borders) {
			std::size_t ntouches = 0;
			for (const auto& prev_border : prev_borders) {
				if ((prev_border & border).count() > 0)
					++ntouches;
			}

			if (ntouches == 0)
				++nregions;
			else if (ntouches > 1)
				nregions -= ntouches - 1;
		}

		prev_borders = std::move(curr_borders);
	}

	return nregions;
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
	for (const auto& i : dense_hash) {
		if (i < 16)
			oss << 0;
		oss << i;
	}

	/*std::cout << std::hex;
	for (const auto& i : dense_hash)
		std::cout << i << ' ';
	std::cout << std::dec << '\n';

	std::cout << std::hex << 5 << std::dec << '\n';*/

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