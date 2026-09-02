#include <sstream>
#include <numeric>

#include "common.hpp"

std::vector<ull> get_invalids(ull left, ull right) noexcept
{
	std::vector<ull> inv;

	for (auto i = left; i <= right; i++) {
		auto s = std::to_string(i);
		auto sz = s.size();

		if (sz % 2)
			continue;

		auto hi = s.substr(0, sz / 2);
		auto lo = s.substr(sz / 2);

		if (hi == lo)
			inv.emplace_back(i);
	}

	return inv;
}


int main(int argc, char *argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;
	file >> record;

	std::stringstream ss(record);
	std::string token;

	auto result = 0ULL;
	while (std::getline(ss, token, ',')) {
		auto [left, right] = aoc::parse_pair<ull>(token);
		auto inv = get_invalids(left, right);
		//std::cout << inv << std::endl;
		result += std::accumulate(inv.begin(), inv.end(), 0ULL);
	}

	std::cout << "Result: " << result << std::endl;

	return 0;
}
