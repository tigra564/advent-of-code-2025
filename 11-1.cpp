#include <functional>

#include "11-common.hpp"

int main(int argc, char *argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);

	Connections connections = parse_connections(file);

	std::map<std::string, int> node_paths;

	std::function<int(std::set<std::string>, const std::string&)> count_paths;
	count_paths = [
		&connections, &node_paths, &count_paths
	](std::set<std::string> nodes_visited,  const std::string from)
	{
		if (from == "out")
			return 1;

		auto [visited_it, visited_inserted] = nodes_visited.insert(from);
		// Node already visited
		if (!visited_inserted)
			return 0;

		// Important! We must check for prior visits in this branch
		// before addressing cached values which may be not completely
		// calculated at the point.

		auto [node_paths_it, node_paths_inserted] = node_paths.insert({from, 0});
		if (!node_paths_inserted)
			// Return cached result
			return node_paths_it->second;

		int count = 0;
		for (const auto& to : connections[from]) {
			count += count_paths(nodes_visited, to);
		}

		node_paths_it->second = count;
		return count;
	};

	auto result = count_paths({}, "you");
	std::cout << "Result: " << result << std::endl;

	return 0;
}
