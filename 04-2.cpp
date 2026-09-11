#include <algorithm>

#include "common.hpp"

int remove(std::vector<std::string>& grid) noexcept
{
	constexpr int threshold = 4;
	const std::vector<std::pair<int, int>> directions = {
		{-1, -1}, {-1, 0}, {-1, 1},
		{ 0, -1},          { 0, 1},
		{ 1, -1}, { 1, 0}, { 1, 1}
	};

	auto count = 0;
	for (std::size_t i = 1; i < grid.size() - 1; i++) {
		for (std::size_t j = 1; j < grid[i].size() - 1; j++) {
			if (grid[i][j] != '@') {
				continue;
			}

			auto num = std::count_if(directions.begin(), directions.end(),
				[&grid, i, j](const auto& dir) -> bool {
					auto ch = grid[i + dir.first][j + dir.second];
					return ch == '@' || ch == 'X';
				}
			);

			if (num < threshold) {
				count++;
				grid[i][j] = 'X';
			}
		}
	}

	for (std::size_t i = 1; i < grid.size() - 1; i++) {
		for (std::size_t j = 1; j < grid[i].size() - 1; j++) {
			if (grid[i][j] == 'X')
				grid[i][j] = '.';
		}
	}

	return count;
}


int main(int argc, char* argv[])
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

	auto result = 0;
	while (auto num = remove(grid)) {
		result += num;
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
