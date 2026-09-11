#include <boost/icl/interval_set.hpp>
#include <numeric>

#include "common.hpp"

int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	boost::icl::interval_set<i64> fresh_ranges;

	while (std::getline(file, record)) {
		if (0 == record.size())
			break;

		auto [left, right] = aoc::parse_pair<i64>(record);
		fresh_ranges.insert(boost::icl::discrete_interval<i64>::closed(left, right));
	}

	auto result = std::accumulate(
		fresh_ranges.begin(), fresh_ranges.end(), i64{0},
		[](i64 sum, const auto& interval) {
			return sum + boost::icl::length(interval);
		}
	);

	std::cout << "Result: " << result << '\n';

	return 0;
}
