
/*
This is a solution to both parts of the Day 7 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/6.
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include <list>
#include <functional>
#include <numeric>

std::string solve_part_one(std::ifstream&);
int solve_part_two(std::ifstream&);

int main()
{

	std::string fname;
	std::cout << "This program solves both parts of the Day 7 puzzle of Advent of Code 2017\n";
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

std::string solve_part_one(std::ifstream& ifs)
{
	std::unordered_map<std::string, std::string> tree;
	for (std::string line; std::getline(ifs, line);) {
		std::istringstream iss(line);

		std::string parent;
		iss >> parent;

		for (std::string child; iss >> child;) {
			if (child[0] == '(' || child[0] == '-')
				continue;
			if (child.back() == ',')
				child.pop_back();
			tree[child] = parent;
		}
	}

	auto parent = tree.begin()->first;
	while (tree[parent] != "")
		parent = tree[parent];

	return parent;
}

int solve_part_two(std::ifstream& ifs)
{
	std::unordered_map<std::string, std::pair<int, std::vector<std::string>>> tree;
	for (std::string line; std::getline(ifs, line);) {
		std::istringstream iss(line);

		std::string parent;
		iss >> parent;

		std::pair<int, std::vector<std::string>> data;
		for (std::string child; iss >> child;) {
			if (child[0] == '(') {
				child.pop_back();
				std::istringstream(child.substr(1)) >> data.first;
			}
			else if (child[0] == '-')
				continue;
			else {
				if (child.back() == ',')
					child.pop_back();
				data.second.push_back(child);
			}
		}
		tree[parent] = data;
	}


	// Calculate the weight of the tree recursively. Flag and store the imbalanced node when it is discovered
	// during this process
	bool found_unbalanced_node = false;
	int rebalanced_weight = 0;
	std::function<int(std::string)> calculate_weight;

	calculate_weight = [&](std::string node) {
		std::unordered_map<int, std::vector<std::string>> weights;
		for (const auto& n : tree[node].second)
			weights[calculate_weight(n)].push_back(n);

		if (!found_unbalanced_node && weights.size() > 1) { // Potential imbalance
			for (const auto& p : weights) {
				if (p.second.size() == 1) {
					found_unbalanced_node = true;
					for (const auto& pp : weights) {
						if (p.first != pp.first) {
							int weight_diff = p.first-pp.first;
							rebalanced_weight = tree[p.second.back()].first-weight_diff;
						}
					}
				}
			}
		}

		int node_weight = tree[node].first;
		for (const auto& p : weights) {
			node_weight += p.first*p.second.size();
		}

		return node_weight;
	};

	ifs.clear();
	ifs.seekg(0, std::ios::beg);
	auto root = solve_part_one(ifs); // Begin at root node
	std::cout << calculate_weight(root);

	return rebalanced_weight;
}