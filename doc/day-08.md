# Day 8: Playground

## Part 1

The task is a classic union-find problem. We have a set of boxes, each with three coordinates. We need to connect the closest pairs first, then find the three largest clusters and multiply their sizes.

The first step is to parse the input. Each line is a triple `x,y,z`. We store them as `std::array<i64, 3>`.

```c++
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
```

Then we compute all pairwise squared distances. We do not take the square root: comparing squared distances is equivalent, and it avoids floating-point arithmetic.

```c++
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
```

The distances are stored in a vector of `jb_dist`, which holds the distance and the indices of the two boxes. The `operator<` for `jb_dist` sorts by distance first, then by the indices. This gives a deterministic order when distances are equal.

For Part 1, we only need the first `nconns` distances: 10 for the sample, 1000 for the real input. The code handles both with a single expression.

```c++
int nconns = jbsz >= 1000 ? 1000 : 10;
std::partial_sort(distances.begin(), distances.begin() + nconns, distances.end());
distances.resize(nconns);
```

Then we form the clusters. The union-find is implemented with a `moved_to` field: each cluster points to the cluster it was merged into, or `-1` if it is a root.

```c++
struct Cluster {
	std::vector<int> points;
	int moved_to = -1;
};
```

We start with one cluster per box, each containing only its own index.

```c++
Clusters clusters;

for (std::size_t i = 0; i < max_jboxes; i++) {
	clusters[i] = Cluster();
	clusters[i].points.emplace_back(i);
}
```

When merging two clusters, we always attach the one with the larger index to the one with the smaller index. This keeps the tree shallow. We also update `moved_to` for the original endpoints, which is a form of partial path compression.

```c++
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
	ptshi.clear();
	clusters[hi].moved_to = lo;

	if (d.first != lo)
		clusters[d.first].moved_to = lo;

	if (d.second != lo)
		clusters[d.second].moved_to = lo;

	if (max_jboxes == clusters[lo].points.size())
		break;
}
```

The union-find here uses partial path compression: when merging, we update `moved_to` for the two endpoints of the current edge, but not for the intermediate nodes on the path to the root. A full path compression would update all nodes on the path, giving amortised *O*(α(*N*)) per operation. With partial compression, the amortised cost is *O*(log *N*) on average. For the given input, this is fast enough.

After all merges, we collect the sizes of the clusters, sort them in descending order, and multiply the top three.

```c++
std::partial_sort(sizes.begin(), sizes.begin() + 3, sizes.end(),
	[](auto a, auto b) { return a > b; }
);

auto result = sizes[0] * sizes[1] * sizes[2];
```

**Complexity estimation**

Computing all pairwise distances takes *O*(*N*²), where *N* is the number of boxes. Sorting the distances takes *O*(*N*² log *N*) for the full sort in Part 2, or *O*(*N*²) for the partial sort in Part 1. The union-find operations take *O*(*N*² log *N*) in the worst case with partial path compression. The overall complexity is *O*(*N*² log *N*).

## Part 2

Part 2 asks for the product of the X-coordinates of the two boxes that are connected last—the edge that joins the final two clusters into one.

The algorithm is the same as in Part 1, except that we sort all distances, not just the first `nconns`. We process the distances one by one until all boxes belong to a single cluster. The last processed edge is the answer.

```c++
auto [clusters, last_distance] = form_clusters(distances, jbsz);
```

The `form_clusters` function returns the last processed distance. When the last two clusters are merged, the loop breaks, and the iterator points to that edge.

```c++
auto it = distances.begin();
for (; it != distances.end(); it++) {
	const auto& d = *it;
	int lo = d.first, hi = d.second;

	// ...

	if (max_jboxes == clusters[lo].points.size())
		break;
}

return std::make_pair(clusters, *it);
```

**Complexity estimation**

Same as Part 1, except that we sort all *O*(*N*²) distances instead of a partial sort. The overall complexity is *O*(*N*² log *N*).

[<< To Day 7](day-07.md)   [To Day 9 >>](day-09.md)
