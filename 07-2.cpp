#include <unordered_map>

#include "common.hpp"

using Key = std::pair<int, int>;
struct KeyHash {
	std::size_t operator()(const Key& k) const noexcept
	{
		return (static_cast<ull>(k.first) << 32) + k.second;
	}
};

using Memo = std::unordered_map<Key, ull, KeyHash>;

ull num_paths_impl(const std::vector<std::string>& diagram, int row, int col, Memo& memo) noexcept
{
	Key key = {row, col};
	if (auto it = memo.find(key); it != memo.end()) {
		return it->second;
	}

	for (size_t i = row; i < diagram.size(); i++) {
		auto line = diagram[i];
		if ('^' != line[col])
			continue;

		ull left = num_paths_impl(diagram, i, col-1, memo);
		ull right = num_paths_impl(diagram, i, col+1, memo);

		auto result = left + right;
		memo[key] = result;
		return result;
	}

	memo[key] = 1;
	return 1;
}


ull num_paths(const std::vector<std::string> &diagram, int row, int col) noexcept
{
	Memo memo;
	return num_paths_impl(diagram, row, col, memo);
}


int main(int argc, char *argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	std::vector<std::string> diagram;

	while (file >> record) {
		diagram.emplace_back(record);
	}

	int row = 2;
	int col = diagram[0].find('S');
	auto result = num_paths(diagram, row, col);

	std::cout << "Result: " << result << std::endl;

	return 0;
}
