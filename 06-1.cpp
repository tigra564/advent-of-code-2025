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

	auto nproblems = 0U;
	while (std::getline(file, record)) {
		//std::cout << record << '\n';
		std::string field;

		if (0 == nproblems) {
			std::stringstream probe_rec(record);
			while (probe_rec >> field)
				nproblems++;

			numbers.resize(nproblems, std::vector<i64>());
			ops.reserve(nproblems);
		}

		std::stringstream rec(record);
		auto i = 0;
		while (rec >> field) {
			auto sym = field[0];
			switch (sym) {
			case '*':
				ops.emplace_back(1ULL, std::multiplies<i64>());
				break;
			case '+':
				ops.emplace_back(0ULL, std::plus<i64>());
				break;
			default:
				numbers[i].emplace_back(std::stoll(field));
			}
			i++;
		}
	}

	auto result = 0LL;
	std::vector<i64> partials;
	for (std::size_t i = 0; i < nproblems; i++) {
		result += std::accumulate(numbers[i].begin(), numbers[i].end(),
			ops[i].first, ops[i].second);
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
