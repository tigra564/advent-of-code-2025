#include <algorithm>

#include "common.hpp"

int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	constexpr int ndigits = 12;

	auto result = i64{0};
	while (file >> record) {
		auto joltage = i64{0};

		auto start = record.begin();
		for (auto i = ndigits - 1; i >= 0; i--) {
			auto digit_it = std::max_element(start, record.end() - i);
			joltage = joltage * 10 + (*digit_it - '0');
			start = digit_it + 1;
		}

		result += joltage;
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
