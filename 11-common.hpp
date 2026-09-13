#ifndef AOC_11_COMMON_HPP
#define AOC_11_COMMON_HPP

#include "common.hpp"

using Connections = std::map<std::string, std::set<std::string>>;

Connections parse_connections(std::ifstream& file);

#endif //AOC_11_COMMON_HPP
