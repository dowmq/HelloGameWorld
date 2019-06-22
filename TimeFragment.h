#pragma once
#include <chrono>

using namespace std::chrono;

class TimeFragment
{
public:
	TimeFragment() : time{ steady_clock::now() } {}
	TimeFragment(const TimeFragment& before) : time{ before.time } {}
	TimeFragment& operator=(const TimeFragment& before) { time = before.time; }
	TimeFragment& operator=(TimeFragment&& before) { time = std::move(before.time); return *this; }

	void		Update() { time = steady_clock::now(); }
	double		operator-(TimeFragment& before) {
					return (time - before.time).count();
	}
public:
	std::chrono::time_point<steady_clock> time;
};