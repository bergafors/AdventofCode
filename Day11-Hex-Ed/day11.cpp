/*
This is a solution to both parts of the Day 11 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/11.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

int solve_part_one(std::ifstream&);
int solve_part_two(std::ifstream&);
int dist(std::unordered_map<std::string, int>&);

int main()
{

	std::string fname;
	std::cout << "This program solves both parts of the Day 11 puzzle of Advent of Code 2017\n";
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
	std::unordered_map<std::string, int> path;
	for (std::string line; std::getline(ifs, line);) {
		std::istringstream linestream(line);
		std::string val;

		while (std::getline(linestream, val, ','))
			++path[val];
	}

	return dist(path);
}

int solve_part_two(std::ifstream& ifs)
{
	int max_dist = 0;
	std::unordered_map<std::string, int> path;
	for (std::string line; std::getline(ifs, line);) {
		std::istringstream linestream(line);
		std::string val;

		while (std::getline(linestream, val, ',')) {
			++path[val];
			max_dist = std::max(max_dist, dist(path));
		}
	}
	return max_dist;
}

int dist(std::unordered_map<std::string, int>& path)
{
	// Choose ne-sw and se-nw as the cardinal directions. Separate
	// movement along s-n into combinations of movement along these
	// directions.
	std::unordered_map<std::string, int> cardinal_path;
	cardinal_path["ne"] = path["ne"] + path["n"];
	cardinal_path["nw"] = path["nw"] + path["n"];
	cardinal_path["se"] = path["se"] + path["s"];
	cardinal_path["sw"] = path["sw"] + path["s"];

	// Calculate the net movement along the cardinal directions.
	std::unordered_map<std::string, int> net_path;
	net_path["ne"] = cardinal_path["ne"] - cardinal_path["sw"];
	net_path["se"] = cardinal_path["se"] - cardinal_path["nw"];

	// If the movement along ne-sw and se-nw have the same sign the total distance
	// is just the sum of each contribution. If they have different sign, notice
	// that two steps along, for example, ne-sw and nw-se is equal to one step along
	// n-s.
	if (net_path["ne"] * net_path["se"] >= 0)
		return std::abs(net_path["ne"]) + std::abs(net_path["se"]);
	else
		return std::max(std::abs(net_path["ne"]), std::abs(net_path["se"]));
}