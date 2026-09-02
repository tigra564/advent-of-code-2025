#ifndef _11_COMMON_HPP
#define _11_COMMON_HPP

#include <sstream>

#include "common.hpp"

using Connections = std::map<std::string, std::set<std::string>>;

Connections parse_connections(std::ifstream& file);

#endif //_11_COMMON_HPP
