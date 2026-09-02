#include "08-common.hpp"

int main(int argc, char *argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);

	Jboxes jboxes = parse_jboxes(file);
	std::vector<jb_dist> distances = get_distances(jboxes);

	auto jbsz = jboxes.size();

	std::sort(distances.begin(), distances.end());

	auto [clusters, last_distance] = form_clusters(distances, jbsz);

	auto result = jboxes[last_distance.first][0] * jboxes[last_distance.second][0];
	std::cout << "Result: " << result << std::endl;

	return 0;
}
