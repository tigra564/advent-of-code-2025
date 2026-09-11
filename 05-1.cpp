#include "common.hpp"

int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	std::vector<std::pair<i64, i64>> fresh_ranges;

	while (std::getline(file, record)) {
		if (0 == record.size())
			break;

		auto [left, right] = aoc::parse_pair<i64>(record);
		fresh_ranges.emplace_back(left, right);
	}

	auto result = 0;
	while (std::getline(file, record)) {
		i64 id = std::stoll(record);

		for (const auto& range : fresh_ranges) {
			if (range.first <= id && id <= range.second) {
				result++;
				break;
			}
		}
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
