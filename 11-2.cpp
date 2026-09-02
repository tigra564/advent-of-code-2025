#include <functional>

#include "11-common.hpp"

struct PathInfo {
	ll count_empty;
	ll count_dac;
	ll count_fft;
	ll count_both;
};

std::ostream& operator<<(std::ostream& os, const PathInfo& info)
{
	std::cout
		<< "empty: " << info.count_empty
		<< ", dac: " << info.count_dac
		<< ", fft: " << info.count_fft
		<< ", both: " << info.count_both;
	return os;
}

int main(int argc, char *argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);

	Connections connections = parse_connections(file);

	std::map<std::string, PathInfo> node_paths;

	std::function< PathInfo(std::set<std::string>, const std::string&) > count_paths;
	count_paths = [
		&connections, &node_paths, &count_paths
	](std::set<std::string> nodes_visited,  const std::string from)
	{
		if (from == "out")
			return PathInfo{1, 0, 0, 0};

		auto [visited_it, visited_inserted] = nodes_visited.insert(from);
		// Node already visited
		if (!visited_inserted)
			return PathInfo{0, 0, 0, 0};

		// Important! We must check for prior visits in this branch
		// before addressing cached values which may be not completely
		// calculated at the point.

		auto [node_paths_it, node_paths_inserted] = node_paths.insert({from, {0, 0, 0, 0}});
		if(!node_paths_inserted)
			// Return cached result
			return node_paths_it->second;

		PathInfo new_info = {0, 0, 0, 0};
		//std::cout << "FROM: " << from << std::endl;
		for (const auto& to : connections[from]) {
			auto info = count_paths(nodes_visited, to);
			//std::cout << "<- " << info << std::endl;

			if (from == "dac") {
				new_info.count_dac	+= info.count_dac + info.count_empty;
				new_info.count_both	+= info.count_fft + info.count_both;
			} else if (from == "fft") {
				new_info.count_fft	+= info.count_fft + info.count_empty;
				new_info.count_both	+= info.count_dac + info.count_both;
			} else {
				new_info.count_empty	+= info.count_empty;
				new_info.count_dac	+= info.count_dac;
				new_info.count_fft	+= info.count_fft;
				new_info.count_both	+= info.count_both;
			}
		}

		node_paths_it->second = new_info;
		//std::cout << new_info << std::endl;
		return new_info;
	};

	auto info = count_paths({}, "svr");
	auto result = info.count_both;
	std::cout << "Result: " << result << std::endl;

	return 0;
}
