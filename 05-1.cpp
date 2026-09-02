#include "common.hpp"

int main(int argc, char *argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	std::vector<std::pair<ull, ull>> fresh_ranges;

	while (std::getline(file, record)) {
		if (0 == record.size())
			break;

		auto [left, right] = aoc::parse_pair<ull>(record);
		fresh_ranges.emplace_back(left, right);
	}

	auto result = 0ULL;
	while (std::getline(file, record)) {
		//std::cout << record << std::endl;
		ull id = std::stoll(record);

		for (const auto& range : fresh_ranges) {
			if (range.first <= id && id <= range.second) {
				//std::cout << "fresh\n";
				result++;
				break;
			}
		}
	}

	std::cout << "Result: " << result << std::endl;

	return 0;
}
