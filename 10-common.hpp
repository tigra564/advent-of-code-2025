#ifndef AOC_10_COMMON_HPP
#define AOC_10_COMMON_HPP

#include <sstream>

#include "common.hpp"

struct Machine {
	// My input contains not more than 10 indicators per machine, uint is enough
	unsigned int indicators = 0;
	// Primarily for printing machine configuration
	unsigned int num_inds = 0;
	std::vector<unsigned int> buttons;
	std::vector<unsigned int> joltages;

	Machine() = default;
	Machine(const std::string& record);
};

std::ostream& operator<<(std::ostream& os, const Machine& machine);

#endif //AOC_10_COMMON_HPP
