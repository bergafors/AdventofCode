/*
This is a solution to both parts of the Day 12 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/12.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <ios>
#include <algorithm>

std::pair<int, int> solve_both_parts(std::ifstream&);

int main()
{

	std::string fname;
	std::cout << "This program solves both parts of the Day 12 puzzle of Advent of Code 2017\n";
	std::cout << "Provide the input data file path: ";
	std::cin >> fname;

	std::ifstream file;

	try {

		file.exceptions(std::ifstream::failbit);
		file.open(fname);
	}
	catch (const std::ifstream::failure&) {
		std::cout << "Error reading file\n";
	}

	try {
		file.exceptions(std::ifstream::badbit);

		auto ans = solve_both_parts(file);
		std::cout << "The answer to part one is: " << ans.first << '\n';
		std::cout << "The answer to part two is: " << ans.second << '\n';
	}
	catch (const std::ifstream::failure&) {
		std::cout << "Error reading file\n";
	}

	return 0;
}

std::pair<int, int> solve_both_parts(std::ifstream& ifs)
{
	std::unordered_map<int, std::unordered_set<int>> groups;
	for (std::string line; std::getline(ifs, line);) {
		std::unordered_set<int> new_group;
		std::istringstream linestream(line);

		int leading_digit = 0;
		linestream >> leading_digit;
		new_group.insert(leading_digit);

		linestream.seekg(4, std::ios::cur);
		for (std::string digit; std::getline(linestream, digit, ',');)
			new_group.insert(std::stoi(digit));

		groups[leading_digit].insert(new_group.begin(), new_group.end());

		// Find which, if any, existing groups this new group is connected to
		std::vector<int> connected_groups;
		for (const auto& digit : groups[leading_digit]) {
			for (const auto&[k, v] : groups) {
				if (k == leading_digit)
					continue;
				if (auto pos = v.find(digit); pos != v.end()) {
					connected_groups.push_back(k);
					break;
				}
			}
		}

		// Find the largest one of the connected groups. See below for reason.
		auto max_group = leading_digit;
		for (const auto& i : connected_groups) {
			if (groups[i].size() > groups[max_group].size())
				max_group = i;
		}

		// We intend to merge to group[leading_digit]. By swapping the largest group here we ensure
		// the minimal amount of copies and erases.
		groups[leading_digit].swap(groups[max_group]);

		// Merge the connected sets and erase the remaining copies
		for (auto it = groups.begin(); it != groups.end() && !connected_groups.empty();) {
			auto& [k, v] = *it;
			auto pos = std::find(connected_groups.begin(), connected_groups.end(), k);
			if (pos != connected_groups.end()) {
				groups[leading_digit].insert(v.begin(), v.end());
				it = groups.erase(it);
				connected_groups.erase(pos);
			}
			else {
				++it;
			}
		}
	}

	std::size_t group_size = 0;
	for (const auto& p : groups) {
		const auto& v = p.second;
		if (auto it = v.find(0); it != v.end()) {
			group_size = v.size();
			break;
		}
	}

	auto ngroups = groups.size();

	return { group_size, ngroups };
}
