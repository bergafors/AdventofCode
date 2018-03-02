/*
This is a solution to both parts of the Day 20 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/20.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>

#include <vector>
#include <tuple>
#include <algorithm>
#include <iterator>

struct particle
{
	using _3tuple = std::tuple<int, int, int>;

	_3tuple p;
	_3tuple v;
	_3tuple a;

	particle(_3tuple _p, _3tuple _v, _3tuple _a) : p(_p), v(_v), a(_a) {};
};

int solve_part_one(const std::vector<particle>&);
int solve_part_two(const std::vector<particle>&);
std::vector<particle> read_input(std::ifstream&);
int initial_state_norm(const std::tuple<int, int, int>&, const std::tuple<int, int, int>&);

int main()
{

	std::string fname;;
	std::cout << "This program solves both parts of the Day 19 puzzle of Advent of Code 2017\n";
	std::cout << "Enter the input data file path: ";
	std::cin >> fname;

	std::ifstream file;
	try {
		file.exceptions(std::ifstream::failbit);
		file.open(fname);

	}
	catch (const std::ifstream::failure&) {
		std::cout << "Error opening file\n";
		return 0;
	}

	std::vector<particle> particle_data;
	try {
		file.exceptions(std::ifstream::badbit);
		particle_data = read_input(file);
	}
	catch (std::ifstream::failure&) {
		std::cout << "Error reading file\n";
		return 0;
	}

	std::cout << "The answer to part one is: " << solve_part_one(particle_data) << '\n';
	std::cout << "The answer to part two is: " << solve_part_two(particle_data) << '\n';

	return 0;
}

std::vector<particle> read_input(std::ifstream& file)
{
	std::vector<particle> particle_data;
	for (std::string line; std::getline(file, line);) {
		std::istringstream linestream(line);

		std::tuple<int, int, int> p, v, a;
		std::string data_block;

		linestream >> data_block;
		sscanf_s(data_block.c_str(), "p=<%d,%d,%d>", &std::get<0>(p), &std::get<1>(p), &std::get<2>(p));

		linestream >> data_block;
		sscanf_s(data_block.c_str(), "v=<%d,%d,%d>", &std::get<0>(v), &std::get<1>(v), &std::get<2>(v));

		linestream >> data_block;
		sscanf_s(data_block.c_str(), "a=<%d,%d,%d>", &std::get<0>(a), &std::get<1>(a), &std::get<2>(a));

		particle_data.emplace_back(std::move(p), std::move(v), std::move(a));
	}

	return particle_data;
}

int solve_part_one(const std::vector<particle>& particle_data)
{
	auto acceleration_comp = [](const auto& pa, const auto& pb) {
		return initial_state_norm(pa.a, pa.a) < initial_state_norm(pb.a, pb.a);
	};
	auto it_min_acc = std::min_element(particle_data.begin(), particle_data.end(), acceleration_comp);

	// The particles with minimum acceleration w.r.t the manhattan norm
	// will eventually be closest to the origin
	std::vector<decltype(it_min_acc)> min_acc_particles;
	for (auto it = particle_data.begin(); it != particle_data.end(); ++it) {
		if (!acceleration_comp(*it, *it_min_acc) && !acceleration_comp(*it_min_acc, *it))
			min_acc_particles.push_back(it);
	}

	// Note that initial_state_norm can be negative if arg 1 != arg 2. While
	// acceleration_comp compares the manhattan norm, velocity_comp 
	// (and position_comp; see below) basically compares both the 
	// manhattan norm of each particle's velocity, as well as the extent
	// that the velocity is anti-parallell to the acceleration.
	auto velocity_comp = [](const auto& ita, const auto& itb) {
		return initial_state_norm(ita->a, ita->v) < initial_state_norm(itb->a, itb->v);
	};
	auto it_min_vec = std::min_element(min_acc_particles.begin(), min_acc_particles.end(), velocity_comp);

	// Given the particles with minimun acceleration, the ones that are minimal w.r.t
	// velocity_comp will eventually be closest to the origin
	decltype(min_acc_particles) min_vec_particles;
	for (auto it = min_acc_particles.begin(); it != min_acc_particles.end(); ++it) {
		if (!velocity_comp(*it, *it_min_vec) && !velocity_comp(*it_min_vec, *it))
			min_vec_particles.push_back(*it);
	}

	auto position_comp = [](const auto& ita, const auto& itb) {
		return initial_state_norm(ita->a, ita->p) < initial_state_norm(itb->a, itb->p);
	};
	auto it_min_pos = std::min_element(min_vec_particles.begin(), min_vec_particles.end(), position_comp);

	// Again, given the filtered particles, the ones that are minimal w.r.t
	// position_comp will eventually be closest to the origin. If there are more than
	// one of these, there is no unique solution.
	for (auto it = min_vec_particles.begin(); it != min_vec_particles.end(); ++it) {
		if (it == it_min_pos)
			continue;
		if (!position_comp(*it, *it_min_pos) && !position_comp(*it_min_pos, *it)) {
			std::cout << "Error: no unique solution in part one.\n";
			break;
		}
	}

	return std::distance(particle_data.begin(), *it_min_pos);
}

// Calculates an obscure value that somehow solves part one
int initial_state_norm(const std::tuple<int, int, int>& a, const std::tuple<int, int, int>& b) 
{
	using namespace std;

	int norm = 0;
	if (get<0>(a) * get<0>(b) < 0)
		norm -= abs(get<0>(b));
	else
		norm += abs(get<0>(b));

	if (get<1>(a) * get<1>(b) < 0)
		norm -= abs(get<1>(b));
	else
		norm += abs(get<1>(b));

	if (get<2>(a) * get<2>(b) < 0)
		norm -= abs(get<2>(b));
	else
		norm += abs(get<2>(b));
		
	return norm;
}


int solve_part_two(const std::vector<particle>& particle_data)
{
	return 0;
}
