#include <numeric>

#include "common.hpp"

bool intervals_overlap(const std::pair<ull, ull>& one, const std::pair<ull, ull>& two) noexcept
{
	// Either left of pair one is inside pair two or adjacent to pair two from right or...
	// the same for left of pair two wrt pair one
	if (
		(two.first <= one.first && one.first <= two.second + 1) ||
		(one.first <= two.first && two.first <= one.second + 1)
	)
		return true;
	// The same for right sides
	if (
		(two.first - 1 <= one.second && one.second <= two.second) ||
		(one.first - 1 <= two.second && two.second <= one.second)
	)
		return true;

	return false;
}


std::pair<ull, ull> merge_intervals(const std::pair<ull, ull>& one, const std::pair<ull, ull>& two) noexcept
{
	std::pair<ull, ull> merged = { std::min(one.first, two.first), std::max(one.second, two.second) };
	//std::cout << one << " U " << two << std::endl;
	return merged;
}


int main(int argc, char *argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	std::set<std::pair<ull, ull>> fresh_ranges;

	while (std::getline(file, record)) {
		if (0 == record.size())
			break;

		auto [left, right] = aoc::parse_pair<ull>(record);
		fresh_ranges.emplace(left, right);
	}

	bool merged;
	do {
		merged = false;
		for (auto it = fresh_ranges.begin(); it != fresh_ranges.end() && !merged; ++it) {
			//std::cout << *it << std::endl;
			for (auto jt = std::next(it); jt != fresh_ranges.end() && !merged; ++jt) {
				//std::cout << "   " << *jt << std::endl;
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

	/*
	std::cout << "Finally:" << std::endl;
	for (const auto& range : fresh_ranges) {
		std::cout << range <<std::endl;
	}
	*/

	auto result = std::accumulate(
		fresh_ranges.begin(), fresh_ranges.end(), 0ULL,
		[](ull sum, const auto& range) {
			return sum + range.second - range.first + 1;
		}
	);

	std::cout << "Result: " << result << std::endl;

	return 0;
}
