/*
This is a solution to both parts of the Day 25 (the last day!) puzzle of Advent of Code 2017.

The problem description can be found at https://adventofcode.com/2017/day/25.
*/

#include <iostream>
#include <deque>

enum class State {A, B, C, D, E, F};

const long NSTEPS = 12'919'244;
const State start_state = State::A;

long solve_part_one();

void eval_state(State& state, std::size_t& cursor, std::deque<int>& tape);

int main() 
{

	std::cout << "This program solves both parts of the Day 23 puzzle of Advent of Code 2017\n";
	std::cout << "The answer to part one is: " << solve_part_one() << '\n';

	return 0;
}

long solve_part_one()
{
	std::deque<int> tape{0};
	std::size_t cursor = 0;
	State state = start_state;
	for (long i = 0; i < NSTEPS; ++i)
		eval_state(state, cursor, tape);

	return std::count(tape.begin(), tape.end(), 1);
}

void eval_state(State& state, std::size_t& cursor, std::deque<int>& tape)
{
	if (cursor == 0) {
		tape.push_front(0);
		++cursor;
	}

	if (cursor == tape.size() - 1)
		tape.push_back(0);

	switch (state)
	{
	case State::A:
		if (tape[cursor] == 0) {
			tape[cursor] = 1;
			++cursor;
			state = State::B;
		}
		else {
			tape[cursor] = 0;
			--cursor;
			state = State::C;
		}
		break;
	case State::B:
		if (tape[cursor] == 0) {
			tape[cursor] = 1;
			--cursor;
			state = State::A;
		}
		else {
			tape[cursor] = 1;
			++cursor;
			state = State::D;
		}
		break;
	case State::C:
		if (tape[cursor] == 0) {
			tape[cursor] = 1;
			++cursor;
			state = State::A;
		}
		else {
			tape[cursor] = 0;
			--cursor;
			state = State::E;
		}
		break;
	case State::D:
		if (tape[cursor] == 0) {
			tape[cursor] = 1;
			++cursor;
			state = State::A;
		}
		else {
			tape[cursor] = 0;
			++cursor;
			state = State::B;
		}
		break;
	case State::E:
		if (tape[cursor] == 0) {
			tape[cursor] = 1;
			--cursor;
			state = State::F;
		}
		else {
			tape[cursor] = 1;
			--cursor;
			state = State::C;
		}
		break;
	case State::F:
		if (tape[cursor] == 0) {
			tape[cursor] = 1;
			++cursor;
			state = State::D;
		}
		else {
			tape[cursor] = 1;
			++cursor;
			state = State::A;
		}
		break;
	default:
		break;
	}

	return;
}