#include <sstream>
#include <functional>
#include <numeric>

#include "common.hpp"

int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	std::vector<std::vector<i64>> numbers;
	std::vector<std::pair<i64, std::function<i64(i64, i64)>>> ops;

	std::size_t nproblems = 0;
	while (std::getline(file, record)) {
		std::string field;

		if (0 == nproblems) {
			std::stringstream probe_rec(record);
			while (probe_rec >> field)
				nproblems++;

			numbers.resize(nproblems, std::vector<i64>());
			ops.reserve(nproblems);
		}

		std::stringstream rec(record);
		std::size_t i = 0;
		while (rec >> field) {
			auto sym = field[0];
			switch (sym) {
			case '*':
				ops.emplace_back(i64{1}, std::multiplies<i64>());
				break;
			case '+':
				ops.emplace_back(i64{0}, std::plus<i64>());
				break;
			default:
				numbers[i].emplace_back(std::stoll(field));
			}
			i++;
		}
	}

	auto result = i64{0};
	for (std::size_t i = 0; i < nproblems; i++) {
		result += std::accumulate(numbers[i].begin(), numbers[i].end(),
			ops[i].first, ops[i].second);
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
