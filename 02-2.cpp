#include <sstream>
#include <numeric>

#include "common.hpp"

std::set<ull> get_invalids(ull left, ull right) noexcept
{
	std::set<ull> inv;

	for (auto i = left; i <= right; i++) {
		auto s = std::to_string(i);
		auto sz = s.size();

		for (auto k = 1ULL; k < sz; k++) {
			if (sz % k)
				continue;

			auto patt = s.substr(0, k);
			std::string result;
			result.reserve(sz);
			for (auto p = 0ULL; p < sz / k; p++) {
				result += patt;
			}

			if (result == s)
				inv.emplace(i);
		}
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
