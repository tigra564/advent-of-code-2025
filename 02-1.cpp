#include <sstream>
#include <numeric>

#include "common.hpp"

std::vector<i64> get_invalids(i64 left, i64 right) noexcept
{
	std::vector<i64> inv;

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


int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;
	file >> record;

	std::stringstream ss(record);
	std::string token;

	auto result = i64{0};
	while (std::getline(ss, token, ',')) {
		auto [left, right] = aoc::parse_pair<i64>(token);
		auto inv = get_invalids(left, right);
		result += std::accumulate(inv.begin(), inv.end(), i64{0});
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
