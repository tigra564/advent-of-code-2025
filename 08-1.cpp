#include "08-common.hpp"

int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);

	Jboxes jboxes = parse_jboxes(file);
	std::vector<jb_dist> distances = get_distances(jboxes);

	auto jbsz = jboxes.size();

	// So we can cope both with sample and real input
	int nconns = jbsz >= 1000 ? 1000 : 10;
	std::partial_sort(distances.begin(), distances.begin() + nconns, distances.end());
	distances.resize(nconns);

	auto [clusters, last_distance] = form_clusters(distances, jbsz);

	std::vector<int> sizes;
	sizes.reserve(clusters.size());
	for (const auto& c : clusters) {
		sizes.emplace_back(c.second.points.size());
	}

	std::partial_sort(sizes.begin(), sizes.begin() + 3, sizes.end(),
		[](auto a, auto b) { return a > b; }
	);
	//std::cout << sizes << '\n';

	auto result = sizes[0] * sizes[1] * sizes[2];
	std::cout << "Result: " << result << '\n';

	return 0;
}
