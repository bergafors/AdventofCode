/*
This is a solution to both parts of the Day 18 puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/18.

There's a race condition in the code for recieving data in part two that I couldn't
resolve with the approach I used here

The readability of part one and two (and code duplication between) could be 
improved using a function table instead of large if-else statements. Not to 
mention adding some comments.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <unordered_map>
#include <vector>
#include <deque>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

template<typename T>
struct thread_safe_queue_agg
{
	std::deque<T> queue;
	std::mutex m;
	std::condition_variable cv;
	long long nsent = 0;
};

int solve_part_one(std::ifstream&);
long long solve_part_two(std::string);
void reach_deadlock(std::string,
	thread_safe_queue_agg<long long>& inc_queue, thread_safe_queue_agg<long long>& out_queue,
	bool& this_waiting, bool& that_waiting);

int main()
{

	std::string fname;;
	std::cout << "This program solves both parts of the Day 18 puzzle of Advent of Code 2017\n";
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

	try {
		file.exceptions(std::ifstream::badbit);

		std::cout << "The answer to part one is: " << solve_part_one(file) << '\n';
		std::cout << "The answer to part two is: " << solve_part_two(fname) << '\n';
	}
	catch (std::ifstream::failure&) {
		std::cout << "Error reading file\n";
		return 0;
	}

	return 0;
}

int solve_part_one(std::ifstream& file)
{
	long long last_sound = 0;
	std::unordered_map<std::string, long long> registers;
	std::vector<std::ifstream::pos_type> stream_pos{file.tellg()};
	for (std::string line; std::getline(file, line);) {
		stream_pos.push_back(file.tellg());
		std::istringstream iss(line);

		std::string op_code, sX, sY;
		iss >> op_code >> sX >> sY;

		long long Y = 0;
		if (sY.size() > 0) {
			if (sY[0] >= 'a')
				Y = registers[sY];
			else
				Y = std::stoi(sY);
		}

		if (op_code == "set")
			registers[sX] = Y;
		else if (op_code == "add")
			registers[sX] += Y;
		else if (op_code == "mul")
			registers[sX] *= Y;
		else if (op_code == "mod")
			registers[sX] %= Y;
		else if (op_code == "rcv") {
			if (registers[sX] != 0)
				return static_cast<int>(last_sound);
		}
		else if (op_code == "snd")
			last_sound = registers[sX];
		else if (op_code == "jgz") {
			long long val = (sX[0] >= 'a') ? registers[sX] : std::stoi(sX);
			if (val <= 0)
				continue;
			if (file.eof() && (Y <= -1 || Y >= 2))
				file.clear();
			if (Y < 0) {
				for (int i = 0; i < std::abs(Y) + 1; ++i)
					stream_pos.pop_back();
				file.seekg(stream_pos.back());
			}
			else {
				std::string temp;
				for (int i = 0; i < Y - 1; ++i) {
					std::getline(file, temp);
					stream_pos.push_back(file.tellg());
				}
			}		
		}
		else {
			std::cout << "Error in part one." <<  op_code <<'\n';
			return 0;
		}
	}

	return -1;
}

long long solve_part_two(std::string fname)
{
	bool waiting_a = false, waiting_b = false;
	thread_safe_queue_agg<long long> qa, qb;

	// Use nsent as a vehicle to send a thread id to the threads
	// Reset to zero within reach_deallock
	qb.nsent = 1; 

	std::thread t1(reach_deadlock, fname, std::ref(qa), std::ref(qb), std::ref(waiting_a), std::ref(waiting_b));
	std::thread t2(reach_deadlock, fname, std::ref(qb), std::ref(qa), std::ref(waiting_b), std::ref(waiting_a));

	t1.join();
	t2.join();

	// The number of values sent through the input queue for thread a
	// is the number of values sent by thread b
	return qa.nsent; 
}

void reach_deadlock(std::string fname,
		thread_safe_queue_agg<long long>& inc_queue_agg, thread_safe_queue_agg<long long>& out_queue_agg,
		bool& this_waiting, bool& that_waiting)
{
	std::unordered_map<std::string, long long> registers;
	if (inc_queue_agg.nsent == 1) {
		registers["p"] = 1;
		inc_queue_agg.nsent = 0;
	}

	std::ifstream file(fname);
	std::vector<std::ifstream::pos_type> stream_pos{ file.tellg() };
	for (std::string line; std::getline(file, line);) {
		stream_pos.push_back(file.tellg());
		std::istringstream iss(line);

		std::string op_code, sX, sY;
		iss >> op_code >> sX >> sY;

		long long Y = 0;
		if (sY.size() > 0) {
			if (sY[0] >= 'a')
				Y = registers[sY];
			else
				Y = std::stoi(sY);
		}

		if (op_code == "set")
			registers[sX] = Y;
		else if (op_code == "add")
			registers[sX] += Y;
		else if (op_code == "mul")
			registers[sX] *= Y;
		else if (op_code == "mod")
			registers[sX] %= Y;
		else if (op_code == "rcv") {
			std::unique_lock<std::mutex> lk(inc_queue_agg.m);
			const auto timeout = std::chrono::milliseconds(500);
			while (inc_queue_agg.queue.empty()) {
				this_waiting = true;
				if (that_waiting)
					return;
				inc_queue_agg.cv.wait_for(lk, timeout);		
			}
			this_waiting = false;
			registers[sX] = inc_queue_agg.queue.back();
			inc_queue_agg.queue.pop_back();
		}
		else if (op_code == "snd") {
			std::lock_guard<std::mutex> lk(out_queue_agg.m);
			out_queue_agg.queue.push_front(registers[sX]);
			out_queue_agg.cv.notify_one();
			++out_queue_agg.nsent;
		}
		else if (op_code == "jgz") {
			long long val = (sX[0] >= 'a') ? registers[sX] : std::stoi(sX);
			if (val <= 0)
				continue;
			if (file.eof() && (Y <= -1 || Y >= 2))
				file.clear();
			if (Y < 0) {
				for (int i = 0; i < std::abs(Y) + 1; ++i)
					stream_pos.pop_back();
				file.seekg(stream_pos.back());
			}
			else {
				std::string temp;
				for (int i = 0; i < Y - 1; ++i) {
					std::getline(file, temp);
					stream_pos.push_back(file.tellg());
				}
			}
		}
		else {
			std::cout << "Error in part two." << op_code << '\n';
			return;
		}
	}
}


