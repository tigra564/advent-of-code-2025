#include <sstream>
#include <numeric>

#include "common.hpp"

int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;
	std::vector<std::string> syms;

	while (std::getline(file, record)) {
		syms.emplace_back(record);
	}

	auto result = i64{0};
	std::vector<i64> numbers;
	for (int i = syms[0].size() - 1; i >= 0; i--) {
		bool is_empty = true;
		auto number = i64{0};
		for (std::size_t j = 0; j < syms.size() - 1; j++) {
			auto sym = syms[j][i];
			if (' ' == sym)
				continue;
			is_empty = false;
			number = number * 10 + (sym - '0');
		}
		if (!is_empty)
			numbers.emplace_back(number);

		auto op = syms[syms.size()-1][i];
		if (' ' == op)
			continue;

		i64 partial;
		switch (op) {
		default: continue;
		case '*':
			partial = std::accumulate(numbers.begin(), numbers.end(),
				i64{1}, std::multiplies<i64>());
			break;
		case '+':
			partial = std::accumulate(numbers.begin(), numbers.end(),
				i64{0}, std::plus<i64>());
			break;
		}
		numbers.clear();

		result += partial;
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
