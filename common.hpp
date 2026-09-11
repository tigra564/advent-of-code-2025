#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cstdint> // for std::int64_t

using i64 = std::int64_t;

namespace aoc {
	std::ifstream open_data(int argc, char* argv[]);

	template<typename T>
	std::pair<T, T> parse_pair(const std::string& s, const std::string& delim = "-") {
		std::size_t pos = s.find(delim);

		T first  = static_cast<T>(std::stoll(s.substr(0, pos)));
		T second = static_cast<T>(std::stoll(s.substr(pos + delim.size())));
		return {first, second};
	}
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector) {
	os << '[';
	bool first = true;
	for (const auto& v : vector) {
		if (!first)
			os << ", ";
		first = false;
		os << v;
	}
	os << ']';
	return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& set) {
	os << '{';
	bool first = true;
	for (const auto& v : set) {
		if (!first)
			os << ", ";
		first = false;
		os << v;
	}
	os << '}';
	return os;
}

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& pair) {
	os << '<' << pair.first << ", " << pair.second << '>';
	return os;
}

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::map<T1, T2>& map) {
	for (const auto& [k, v] : map)
		os << k << ": " << v << '\n';
	return os;
}

#endif //COMMON_HPP
