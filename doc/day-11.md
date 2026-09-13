# Day 11: Reactor

## Part 1

Here we implement the same approach as we used for counting paths in Part 2 of Day 7. The only slight difference is that here recursion is implemented with a lambda function, and there is no need to construct a custom hash for the key. In Day 7, the key was a pair of integers, and we had to write a hash function for it. Here the key is a `std::string`, and `std::map` handles it out of the box.

```c++
std::function<i64(std::set<std::string>, const std::string&)> count_paths;
count_paths = [
	&connections, &node_paths, &count_paths
](std::set<std::string> nodes_visited, const std::string from) -> i64
{
	if (from == "out")
		return 1;

	auto visited_inserted = nodes_visited.insert(from).second;
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

	i64 count = 0;
	for (const auto& to : connections[from]) {
		count += count_paths(nodes_visited, to);
	}

	node_paths_it->second = count;
	return count;
};
```

The set of visited nodes is passed by value, so each recursive call gets its own copy. This is necessary because the graph may contain cycles, and we must not revisit a node on the same path.

After Day 9, where nothing foreshadowed that the input would violate HV-convexity, I started making only the assumptions explicitly stated in the puzzle. Since nothing is said about the absence of cycles in the graph, it is logical to assume that they are not just possible but actually present.

Copying the set on each call is not the most efficient approach—a backtracking version with a single set would avoid the copies—but it would also invalidate the memoisation cache, since the result for a node depends on which nodes are already visited. For the given input (588 lines), the copying is fast enough: the first part runs in 5 milliseconds.

**Complexity estimation**

The algorithm traverses the graph once, visiting each node and each edge at most once. However, the set of visited nodes is copied on each recursive call, which adds a factor of *O*(*V*) per call in the worst case. The overall complexity is *O*(*V* × (*V* + *E*)), where *V* is the number of nodes and *E* is the number of edges.

## Part 2

Brute-forcing Part 2 is not an option. In my case, the result amounted to 390,108,778,818,526 (hundreds of trillions!) paths.

Also, we cannot simply copy the approach from Part 1, and here is why. In Part 1, we calculate all paths unconditionally, but in this case a valid path must contain both `fft` and `dac` nodes.

To handle this, it is enough to maintain four counters for each path, tracking which of the required nodes have been seen so far:

* `empty`—if neither `dac` nor `fft` has been seen yet
* `dac`—if at least one `dac` node has been seen, but no `fft` nodes
* `fft`—if at least one `fft` node has been seen, but no `dac` nodes
* `both`—if at least one `dac` and at least one `fft` node have been seen

This could be implemented as a [deterministic finite automaton](https://en.wikipedia.org/wiki/Deterministic_finite_automaton), but simply tracking the counters is enough.

A path is created when we encounter a 1-step connection ending with the `out` label. Initially, all its counters are set to 0, except the `empty` counter, which is set to 1.

For each node, we create a new path with zero counters and iterate over its outgoing connections.

The rules for updating the counters of a path, given the counter values of the paths that a connection points to, are as follows:

* If the current node is `dac`, then for each outgoing connection (to already computed paths):
  * the `dac` count is increased by the sum of the connected path's `dac` and `empty` counters
  * the `both` count is increased by the sum of the connected path's `both` and `fft` counters
* If the current node is `fft`, a similar update is performed:
  * the `fft` count is increased by the sum of the connected path's `fft` and `empty` counters
  * the `both` count is increased by the sum of the connected path's `both` and `dac` counters
* For any other node label, all counters are increased individually.

```c++
struct PathInfo {
	i64 count_empty;
	i64 count_dac;
	i64 count_fft;
	i64 count_both;
};
```

The `PathInfo` structure holds the four counters. Its `operator<<` is used only for debug output.

```c++
std::function<PathInfo(std::set<std::string>, const std::string&)> count_paths;
count_paths = [
	&connections, &node_paths, &count_paths
](std::set<std::string> nodes_visited, const std::string from)
{
	if (from == "out")
		return PathInfo{1, 0, 0, 0};

	auto visited_inserted = nodes_visited.insert(from).second;
	// Node already visited
	if (!visited_inserted)
		return PathInfo{0, 0, 0, 0};

	// Important! We must check for prior visits in this branch
	// before addressing cached values which may be not completely
	// calculated at the point.

	auto [node_paths_it, node_paths_inserted] = node_paths.insert({from, {0, 0, 0, 0}});
	if (!node_paths_inserted)
		// Return cached result
		return node_paths_it->second;

	PathInfo new_info{};
	for (const auto& to : connections[from]) {
		auto info = count_paths(nodes_visited, to);

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
	return new_info;
};
```

As in Part 1, the set of visited nodes is passed by value, and the same reasoning applies: the graph may contain cycles, so we must not revisit a node on the same path. The memoisation cache stores `PathInfo` for each node.

**Complexity estimation**

The algorithm has the same structure as in Part 1: it traverses the graph once, with the set of visited nodes copied on each recursive call. The overall complexity is *O*(*V* × (*V* + *E*)), where *V* is the number of nodes and *E* is the number of edges.

For the given input, the algorithm runs in 18 milliseconds.

[<< To Day 10](day-10.md)   [To Day 12 >>](day-12.md)
