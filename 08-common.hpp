#ifndef _08_COMMON_HPP
#define _08_COMMON_HPP

#include <sstream>
#include <unordered_map>
#include <array>
#include <algorithm>

#include "common.hpp"

struct Cluster {
	std::vector<int> points;
	int moved_to = -1;
};

using Clusters = std::unordered_map<int, Cluster>;

std::ostream& operator<<(std::ostream& os, const Clusters& clusters);
	
using Jboxes = std::vector<std::array<i64, 3>>;
Jboxes parse_jboxes(std::ifstream& file);

struct jb_dist {
	i64 distance;
	int first;
	int second;

	bool operator<(const jb_dist& other) const {
		if (distance != other.distance)
			return distance < other.distance;
		if (first != other.first)
			return first < other.first;
		return second < other.second;
	}
};

std::vector<jb_dist> get_distances(const Jboxes& jboxes);
std::pair<Clusters, jb_dist> form_clusters(const std::vector<jb_dist>& distances, std::size_t max_jboxes);

#endif //_08_COMMON_HPP
