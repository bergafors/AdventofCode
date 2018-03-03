/*
This is a solution to both parts of the Day 21 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/21.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

#include <string>
#include <vector>

#include <algorithm>
#include <cassert>

using pattern = std::vector<std::string>;

std::pair<std::size_t, std::size_t> solve_both_parts(std::vector<std::pair<pattern, pattern>>);

std::vector<std::pair<pattern, pattern>> parse_input(std::ifstream&);

std::vector<pattern> split(const pattern&, std::size_t);
pattern assemble(const std::vector<pattern>&);

bool is_variation_of(const pattern&, pattern);
pattern& rotate_pattern(pattern&);
pattern& flip_pattern(pattern&);


int main()
{
	std::string fname;;
	std::cout << "This program solves both parts of the Day 21 puzzle of Advent of Code 2017\n";
	std::cout << "Enter the input data file path: ";
	std::cin >> fname;

	std::ifstream file;
	try {
		file.exceptions(std::ifstream::failbit);
		file.open(fname);
	}
	catch (std::ifstream::failure&) {
		std::cout << "Error opening file.\n";
	}

	std::vector<std::pair<pattern, pattern>> enhancement_rules;
	try {
		file.exceptions(std::ifstream::badbit);
		enhancement_rules = parse_input(file);
	}
	catch (std::ifstream::failure&) {
		std::cout << "Error reading file.\n";
	}
	const auto ans = solve_both_parts(enhancement_rules);
	std::cout << "The answer to part one is: " << ans.first << '\n';
	std::cout << "The answer to part two is: " << ans.second << '\n';


	return 0;
}

std::vector<std::pair<pattern, pattern>> parse_input(std::ifstream& file)
{
	// The number of chars in the largest pattern including the null terminator
	const int MAXSIZE = 20;

	std::vector<std::pair<pattern, pattern>> enhancement_rules;
	for (std::string line; std::getline(file, line);) {
		char input_string[MAXSIZE], output_string[MAXSIZE];
		int i = sscanf_s(line.c_str(), "%s => %s", input_string, MAXSIZE, output_string, MAXSIZE);
		if (i != 2) {
			std::cout << "Error in " __FUNCTION__  << '\n';
			throw;
		}

		pattern input_pattern, output_pattern;

		std::istringstream input_iss(input_string);
		for (std::string row; std::getline(input_iss, row, '/');)
			input_pattern.push_back(row);

		std::istringstream output_iss(output_string);
		for (std::string row; std::getline(output_iss, row, '/');)
			output_pattern.push_back(row);

		enhancement_rules.push_back({input_pattern, output_pattern});
	}

	return enhancement_rules;
}

std::pair<std::size_t, std::size_t> solve_both_parts(std::vector<std::pair<pattern, pattern>> enhancement_rules)
{
	const int NITER_PART_ONE = 5;
	const int NITER_PART_TWO = 20;
	assert(NITER_PART_ONE <= NITER_PART_TWO);

	std::size_t nactive = 0, nactive_part_one = 0;
	// Partition the ehancement rules according to the size of the input pattern
	const auto it_partition = std::partition(enhancement_rules.begin(), enhancement_rules.end(),
		[](const auto& p) {
		return p.first.size() <= 2;
	});

	// Initial pattern
	pattern patt = { ".#.", "..#", "###" };
	for (int i = 0; i < NITER_PART_TWO; ++i) {

		// Increase speed by only examining the enhancement rules
		// whose input pattern size divides the size of %patt, 
		// letting side = 2 take precedence
		std::size_t side = 0;
		decltype(enhancement_rules)::iterator start, finish;
		if (patt.front().size() % 2 == 0) {
			side = 2;
			start = enhancement_rules.begin();
			finish = it_partition;
		}
		else {
			side = 3;
			start = it_partition;
			finish = enhancement_rules.end();
		}

		const std::vector<pattern> split_patterns = split(patt , side);

		// Find the enhancement rule for each entry in %split_patterns and add
		// the output pattern to %enhanced_patterns
		std::vector<pattern> enhanced_patterns;
		enhanced_patterns.reserve(split_patterns.size());
		for (const auto& p : split_patterns) {
			for (auto it = start; it != finish; ++it) {
				if (is_variation_of(p, it->first)) {
					enhanced_patterns.push_back(it->second);
					break;
				}
			}
		}

		patt = assemble(enhanced_patterns);

		if (i == NITER_PART_ONE - 1) {
			for (const auto& str : patt)
				nactive_part_one += std::count(str.begin(), str.end(), '#');
			//nactive_part_one *= nactive_multiplier;
		}
	}

	for (const auto& str : patt)
		nactive += std::count(str.begin(), str.end(), '#');	

	return { nactive_part_one  , nactive};
}

// Split [A] into [B, C, D, E], where the sizes of [B], ..., [E]
// are all side x side
std::vector<pattern> split(const pattern& a, std::size_t side)
{
	const std::size_t nsplits = a.size() / side;

	std::vector<pattern> split_patterns(nsplits * nsplits);
	std::size_t nrows = 0;
	for (const auto& row : a) {
		for (std::size_t i = 0; i < nsplits; ++i) {
			auto start = row.cbegin() + i*side;
			auto finish = start + side;

			// Jump %nsplit ever %side rows
			std::size_t jump = (nrows/ side) * nsplits;
			split_patterns[i + jump].emplace_back(start, finish);
		}
		++nrows;
	}

	return split_patterns;
}

// Assemble [A, B, C, D] into [A, B; C, D], where [A], ..., [D]
// are all square matrices of the same size
pattern assemble(const std::vector<pattern>& patterns)
{	
	const std::size_t old_side = patterns.front().size();
	std::size_t new_side = 0;

	{
		// Total number of chars in the pattern list
		const std::size_t nelem = patterns.size() * old_side * old_side;
		while (new_side * new_side < nelem)
			++new_side;

	}

	// The number of patterns in patterns per side in the reassembled pattern
	std::size_t nblocks = new_side / old_side;

	pattern reassembeld_pattern;
	reassembeld_pattern.reserve(new_side);
	for (std::size_t row = 0; row < new_side; ++row) {
		std::string srow;
		srow.reserve(new_side);
		for (std::size_t col = 0; col < nblocks; ++col)
			srow += patterns[col + (row / old_side) * nblocks][row % old_side];
		reassembeld_pattern.push_back(std::move(srow));
	}

	return reassembeld_pattern;
}

// Check if pattern %a can be turned into %b by a series of flips and rotations
bool is_variation_of(const pattern& a, pattern b)
{
	if (a == b)
		return true;

	for (int i = 0; i < 3; ++i) {
		if (a == rotate_pattern(b))
			return true;
	}

	if (a == flip_pattern(b))
		return true;

	for (int i = 0; i < 3; ++i) {
		if (a == rotate_pattern(b))
			return true;
	}

	return false;
}

pattern& rotate_pattern(pattern& a)
{
	const std::size_t side_len = a.size();

	pattern variation;
	variation.reserve(a.size());
	for (std::size_t i = 0; i < side_len; ++i) {
		std::string row;
		for (std::size_t j = 0; j < side_len; ++j) {
			row.push_back(a[side_len - 1 - j][i]);
		}
		variation.push_back(row);
	}
	a = std::move(variation);
	return a;
}

pattern& flip_pattern(pattern& a)
{
	a = pattern(a.rbegin(), a.rend());
	return a;
}