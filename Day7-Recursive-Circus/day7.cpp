
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

std::string solve_part_one(std::ifstream&);
int solve_part_two(std::ifstream&);
int find_unbalanced_node(bool&, int&, std::string, std::unordered_map<std::string, std::pair<int, std::vector<std::string>>>&);


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

	ifs.clear();
	ifs.seekg(0, std::ios::beg);
	auto root = solve_part_one(ifs); // Begin at root node

	bool found = false;
	int rebalanced_weight = 0;
	find_unbalanced_node(found, rebalanced_weight, root, tree);

	return rebalanced_weight;
}

// Calculate the weight of the tree recursively. Flag the unbalanced node when it is discovered
// during this process and calculate the rebalanced weight.
int find_unbalanced_node(bool& found, int& rebalanced_weight, std::string node, 
	std::unordered_map<std::string, std::pair<int, std::vector<std::string>>>& tree)
{
	std::unordered_map<int, std::vector<std::string>> weights;
	for (const auto& child : tree[node].second) {
		int child_weight = find_unbalanced_node(found, rebalanced_weight, child, tree);
		if (found) // Then we're done and may unwind the recursive weight calculation prematurely.
			return 0;
		weights[child_weight].push_back(child);
	}

	if (!found && weights.size() > 1) { // If weights.size() > 1 then the node is unbalanced
		for (const auto& w : weights) {
			// If a certain weight only corresponds to one child node, that part of the tree must be the unbalanced.
			// As the search is done recursively, the unbalance must occur at that node as well, and not further
			// up the tree.
			if (w.second.size() == 1) {
				found = true;
				auto not_w_it = std::find_if(weights.begin(), weights.end(), [&](const auto& ww) {
					return ww != w;
				});
				int weight_diff = w.first - not_w_it->first;
				rebalanced_weight = tree[w.second.back()].first - weight_diff;
			}
		}
	}

	int node_weight = tree[node].first;
	for (const auto& w : weights)
		node_weight += w.first*w.second.size();

	return node_weight;
}
