#include <sstream>
#include <numeric>

#include "common.hpp"

std::vector<i64> get_invalids(i64 left, i64 right) noexcept
{
	std::vector<i64> inv;

	for (auto i = left; i <= right; i++) {
		auto s = std::to_string(i);
		auto sz = s.size();

		for (std::size_t k = 1; k < sz; k++) {
			if (sz % k)
				continue;

			auto patt = s.substr(0, k);
			std::string candidate;
			candidate.reserve(sz);
			for (std::size_t p = 0; p < sz / k; p++) {
				candidate += patt;
			}

			if (candidate == s) {
				inv.emplace_back(i);
				break;
			}
		}
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
