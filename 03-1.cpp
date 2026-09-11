#include <algorithm>

#include "common.hpp"

int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	auto result = 0ULL;
	while (file >> record) {
		auto first = std::max_element(record.begin(), record.end() - 1);
		auto second = std::max_element(first + 1, record.end());

		result += (*first - '0') * 10 + (*second - '0');

		//std::cout << record << " " << *first << " " << *second << '\n';
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
