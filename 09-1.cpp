#include <array>

#include "common.hpp"

using Tile = std::array<int, 2>;
using Tiles = std::vector<Tile>;


int main(int argc, char *argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	Tiles tiles;

	while (file >> record) {
		//std::cout << record << std::endl;

		auto [t0, t1] = aoc::parse_pair<int>(record, ",");
		tiles.emplace_back(Tile{t0, t1});
	}
	//std::cout << std::endl;

	auto ntiles = tiles.size();

	auto result = 0LL;
	for (size_t i = 0; i < ntiles; i++) {
		for (size_t j = i+1; j < ntiles; j++) {
			auto area =
				static_cast<ll>(std::abs(tiles[i][0] - tiles[j][0]) + 1) *
				static_cast<ll>(std::abs(tiles[i][1] - tiles[j][1]) + 1);
			result = std::max(result, area);
		}
	}

	std::cout << "Result: " << result << std::endl;

	return 0;
}
