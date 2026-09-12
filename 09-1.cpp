#include <array>
#include <algorithm>
#include <cmath>

#include "common.hpp"

using Tile = std::array<int, 2>;
using Tiles = std::vector<Tile>;


int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	Tiles tiles;

	while (file >> record) {
		auto [t0, t1] = aoc::parse_pair<int>(record, ",");
		tiles.emplace_back(Tile{t0, t1});
	}

	auto ntiles = tiles.size();

	auto result = i64{0};
	for (std::size_t i = 0; i < ntiles; i++) {
		for (std::size_t j = i+1; j < ntiles; j++) {
			auto area =
				static_cast<i64>(std::abs(tiles[i][0] - tiles[j][0]) + 1) *
				static_cast<i64>(std::abs(tiles[i][1] - tiles[j][1]) + 1);
			result = std::max(result, area);
		}
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
