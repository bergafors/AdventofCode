/*
This is a solution to both parts of the Day 24 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/24.
*/

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <unordered_map>
#include <list>

#include <algorithm>

using pipe = std::pair<int, int>;

std::vector<pipe> parse_input(std::ifstream& input_file);

// Recursively calculates the strength of the strongest bridge and the strongest strength of the longest bridges
// Returns {max_str, {max_len_max_str, max_len}}, respectively as above.
std::pair<int, std::pair<int, int>> find_maximal_bridges(int connector,
	std::unordered_map<int, std::unordered_map<int, int>>& connection_graph);

// Builds a graph from %pipes. The return value (call it graph) is an unordered_map where graph[i][j]
// is the number of pipes with connections {i, j} and {j, i}. Note the symmetry: graph[i][j] = graph[j][i].
std::unordered_map<int, std::unordered_map<int, int>> build_connection_graph(const std::vector<pipe>& pipes);

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

	std::vector<std::pair<int, int>> pipes;
	try {
		input_file.exceptions(std::ifstream::badbit);
		pipes = parse_input(input_file);
	}
	catch (std::ifstream::failure&) {
		std::cout << "Error reading input_file.\n";
	}

	const auto ans = find_maximal_bridges(0, build_connection_graph(pipes));
	std::cout << "The answer to part one is: " << ans.first << '\n';
	std::cout << "The answer to part two is: " << ans.second.first << '\n';

	return 0;
}

std::vector<std::pair<int, int>> parse_input(std::ifstream& input_file)
{
	std::vector<std::pair<int, int>> pipes;
	for (std::string line; std::getline(input_file, line);) {
		std::replace(line.begin(), line.end(), '/', ' ');
		std::istringstream iss(line);

		std::pair<int, int> pipe;
		iss >> pipe.first >> pipe.second;

		pipes.push_back(pipe);
	}

	return pipes;
}

std::unordered_map<int, std::unordered_map<int, int>> build_connection_graph(const std::vector<pipe>& pipes)
{
	std::unordered_map<int, std::unordered_map<int, int>> connection_graph;
	for (const auto& [i, j] : pipes) {
		++connection_graph[i][j];
		++connection_graph[j][i];
	}

	return connection_graph;
}

std::pair<int, std::pair<int, int>> find_maximal_bridges(int connector, 
	std::unordered_map<int, std::unordered_map<int, int>>& connection_graph)
{
	// {max_str, {max_len_max_str, max_len}}
	std::pair<int, std::pair<int, int>> maximal_values{ 0, {0, 0} };
	for (const auto&[connection, num] : connection_graph[connector]) {
		if (num == 0)
			continue;


		std::pair<int, std::pair<int, int>> added_values{ connection + connector,{ connection + connector, 1 } };

		--connection_graph[connector][connection];
		--connection_graph[connection][connector];

		const auto [s, p] = find_maximal_bridges(connection, connection_graph);

		++connection_graph[connector][connection];
		++connection_graph[connection][connector];

		added_values.first += s;
		maximal_values.first = std::max(maximal_values.first, added_values.first);

		int &len = added_values.second.second,
			&str = added_values.second.first,
			&max_len = maximal_values.second.second,
			&max_len_max_str = maximal_values.second.first;

		str += p.first;
		len += p.second;

		if (max_len < len) {
			max_len = len;
			max_len_max_str = str;
		}
		else if (max_len == len){
			if (max_len_max_str < str)
				max_len_max_str = str;
		}
	}

	return maximal_values;
}