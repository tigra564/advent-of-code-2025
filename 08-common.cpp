#include "08-common.hpp"

std::ostream& operator<<(std::ostream& os, const Clusters& clusters)
{
	for (const auto& p : clusters) {
		os << p.first << ": (" << p.second.moved_to << ")";
		for (const auto& v : p.second.points) {
			os << " " << v;
		}
		os << '\n';
	}

	return os;
}

Jboxes parse_jboxes(std::ifstream& file)
{
	Jboxes jboxes;
	std::string record;

	while (file >> record) {

		std::stringstream ss(record);
		std::array<i64, 3> jbox;
		for (auto i = 0; i < 3; i++) {
			std::string token;
			std::getline(ss, token, ',');
			jbox[i] = std::stoll(token);
		}

		jboxes.emplace_back(jbox);
	}

	return jboxes;
}

std::vector<jb_dist> get_distances(const Jboxes& jboxes)
{
	std::vector<jb_dist> distances;
	auto jbsz = jboxes.size();
	distances.reserve(jbsz * (jbsz-1) / 2);

	for (std::size_t i = 0; i < jbsz; i++) {
		for (std::size_t j = i+1; j < jbsz; j++) {
			i64 dist = 0;
			for (auto k = 0; k < 3; k++) {
				auto diff = jboxes[i][k] - jboxes[j][k];
				dist += diff * diff;
			}
			distances.emplace_back(jb_dist{dist, static_cast<int>(i), static_cast<int>(j)});
		}
	}
	
	return distances;
}

std::pair<Clusters, jb_dist> form_clusters(const std::vector<jb_dist>& distances, std::size_t max_jboxes)
{
	Clusters clusters;

	for (std::size_t i = 0; i < max_jboxes; i++) {
		clusters[i] = Cluster();
		clusters[i].points.emplace_back(i);
	}

	auto it = distances.begin();
	for (; it != distances.end(); it++) {
		const auto& d = *it;
		int lo = d.first, hi = d.second;

		while (-1 != clusters[lo].moved_to)
			lo = clusters[lo].moved_to;

		while (-1 != clusters[hi].moved_to)
			hi = clusters[hi].moved_to;

		if (lo == hi)
			continue;

		if (lo > hi)
			std::swap(lo, hi);

		auto& ptslo = clusters[lo].points;
		auto& ptshi = clusters[hi].points;

		ptslo.insert(ptslo.end(), ptshi.begin(), ptshi.end());
		ptshi.resize(0);
		clusters[hi].moved_to = lo;

		if (d.first != lo)
			clusters[d.first].moved_to = lo;

		if (d.second != lo)
			clusters[d.second].moved_to = lo;

		if (max_jboxes == clusters[lo].points.size())
			break;
	}

	return std::make_pair(clusters, *it);
}
