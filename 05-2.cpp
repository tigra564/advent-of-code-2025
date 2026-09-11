#include <numeric>
#include <algorithm> // for std::min/max
#include <iterator> // for std::next

#include "common.hpp"

bool intervals_overlap(const std::pair<i64, i64>& one, const std::pair<i64, i64>& two) noexcept
{
	return std::max(one.first, two.first) <= std::min(one.second, two.second) + 1;
}


std::pair<i64, i64> merge_intervals(const std::pair<i64, i64>& one, const std::pair<i64, i64>& two) noexcept
{
	std::pair<i64, i64> merged = { std::min(one.first, two.first), std::max(one.second, two.second) };
	return merged;
}


int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	std::set<std::pair<i64, i64>> fresh_ranges;

	while (std::getline(file, record)) {
		if (0 == record.size())
			break;

		auto [left, right] = aoc::parse_pair<i64>(record);
		fresh_ranges.emplace(left, right);
	}

	bool merged;
	do {
		merged = false;
		for (auto it = fresh_ranges.begin(); it != fresh_ranges.end() && !merged; ++it) {
			for (auto jt = std::next(it); jt != fresh_ranges.end() && !merged; ++jt) {
				if (intervals_overlap(*it, *jt)) {
					auto new_interval = merge_intervals(*it, *jt);
					// jt > it always
					fresh_ranges.erase(jt);
					fresh_ranges.erase(it);
					fresh_ranges.insert(new_interval);

					merged = true;
				}
			}
		}
	} while (merged);

	auto result = std::accumulate(
		fresh_ranges.begin(), fresh_ranges.end(), i64{0},
		[](i64 sum, const auto& range) {
			return sum + range.second - range.first + 1;
		}
	);

	std::cout << "Result: " << result << '\n';

	return 0;
}
