#include <boost/icl/interval_set.hpp>
#include <numeric>

#include "common.hpp"

int main(int argc, char *argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	boost::icl::interval_set<ull> fresh_ranges;

	while (std::getline(file, record)) {
		if (0 == record.size())
			break;

		auto [left, right] = aoc::parse_pair<ull>(record);
		fresh_ranges.insert(boost::icl::discrete_interval<ull>::closed(left, right));
	}

	auto result = std::accumulate(
		fresh_ranges.begin(), fresh_ranges.end(), 0ULL,
		[](size_t sum, const auto& interval) {
			return sum + boost::icl::length(interval);
		}
	);

	std::cout << "Result: " << result << std::endl;

	return 0;
}
