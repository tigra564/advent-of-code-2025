#include <algorithm>

#include "common.hpp"

int main(int argc, char *argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	std::vector<std::string> grid;

	file >> record;
	grid.emplace_back(std::string(2 + record.size(), '.'));
	do {
		grid.emplace_back('.' + record + '.');
	} while (file >> record);
	grid.push_back(grid[0]);

	std::vector<std::pair<int, int>> directions = {
		{-1, -1}, {-1, 0}, {-1, 1},
		{ 0, -1},          { 0, 1},
		{ 1, -1}, { 1, 0}, { 1, 1}
	};

	auto result = 0ULL;
	for (size_t i = 1; i < grid.size() - 1; i++) {
		for (size_t j = 1; j < grid[i].size() - 1; j++) {
			if (grid[i][j] != '@') {
				//std::cout << '.';
				continue;
			}

			auto num = std::count_if(directions.begin(), directions.end(),
				[&grid, i, j](const auto& dir) -> bool {
					return grid[i + dir.first][j + dir.second] == '@';
				}
			);

			if (num < 4)
				result++;
			//std::cout << num;
		}
		//std::cout << std::endl;
	}

	std::cout << "Result: " << result << std::endl;

	return 0;
}
