#include <sstream>
#include <numeric>

#include "common.hpp"

int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;
	std::vector<std::string> syms;

	while (std::getline(file, record)) {
		//std::cout << "[" << record << "]" << '\n';
		syms.emplace_back(record);
	}

	auto result = 0ULL;
	std::vector<i64> numbers;
	for (int i = syms[0].size() - 1; i >= 0; i--) {
		bool is_empty = true;
		auto number = 0ULL;
		for (auto j = 0U; j < syms.size() - 1; j++) {
			auto sym = syms[j][i];
			if (' ' == sym)
				continue;
			is_empty = false;
			number *= 10;
			number += sym - '0';
		}
		if (!is_empty)
			numbers.emplace_back(number);
		//std::cout << number << " ";

		auto op = syms[syms.size()-1][i];
		if (' ' == op)
			continue;

		//std::cout << op << '\n';
		i64 partial;
		switch (op) {
		default: continue;
		case '*':
			partial = std::accumulate(numbers.begin(), numbers.end(),
				1ULL, std::multiplies<i64>());
			break;
		case '+':
			partial = std::accumulate(numbers.begin(), numbers.end(),
				0ULL, std::plus<i64>());
			break;
		}
		//std::cout << partial << '\n';
		numbers.resize(0);

		result += partial;
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
