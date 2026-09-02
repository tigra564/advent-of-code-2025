#ifndef _10_COMMON_HPP
#define _10_COMMON_HPP

#include <sstream>
#include <functional>
#include <numeric>
#include <algorithm>
#include <limits>

#include "common.hpp"

struct Machine {
	// My input contains not more than 10 indicators per machine, uint is enough
	unsigned int indicators = 0;
	// Primarily for printing machine configuration
	unsigned int num_inds = 0;
	std::vector<unsigned int> buttons;
	std::vector<unsigned int> joltages;

	Machine() = default;
	// support moving
	Machine(Machine&&) = default;
	Machine& operator=(Machine&&) = default;
	// support copying
	Machine(const Machine&) = default;
	Machine& operator=(const Machine&) = default;

	Machine(const std::string& record);
};

std::ostream& operator<<(std::ostream& os, const Machine& machine);

#endif //_10_COMMON_HPP
