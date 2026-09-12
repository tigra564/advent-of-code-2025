# Day 5: Cafeteria

## Part 1

This part is trivial. We read in all the ranges and then, for each supplied ID, check if it is within the boundaries of any of those ranges. The ranges are closed, hence we use ≤ and ≥.

```c++
for (const auto& range : fresh_ranges) {
	if (range.first <= id && id <= range.second) {
		result++;
		break;
	}
}
```

**Complexity estimation**

For each ID, we scan all ranges. The complexity is *O*(*N* × *M*), where *N* is the number of IDs and *M* is the number of ranges. For the given input, *M* = 187 and *N* is a few thousand, so this runs fast.

## Part 2

Here things start to get complicated. The problem itself is simply a task of merging all the supplied ranges and printing out the total number of elements they contain. With `boost::icl::interval_set` (see [Boost.Icl](https://www.boost.org/doc/libs/release/libs/icl/doc/html/index.html)), this can be done in a couple of minutes. Check the code in [`05-2-boost.cpp`](../05-2-boost.cpp). But without a dedicated data structure, there is some work to do.

My input file contains only 187 ranges. Even for an interpreted language, this is not a big deal. We can brute-force the solution by comparing every pair of ranges. At each pass, we reduce the number of ranges by exactly one—the `merged` flag stops the loops after the first merge.

```c++
if (intervals_overlap(*it, *jt)) {
	auto new_interval = merge_intervals(*it, *jt);
	// jt > it always
	fresh_ranges.erase(jt);
	fresh_ranges.erase(it);
	fresh_ranges.insert(new_interval);

	merged = true;
}
```

The overlap check can be expressed in a single line: two intervals overlap or are adjacent if the maximum of their left ends is no greater than the minimum of their right ends plus one.

```c++
bool intervals_overlap(const std::pair<i64, i64>& one, const std::pair<i64, i64>& two) noexcept
{
	return std::max(one.first, two.first) <= std::min(one.second, two.second) + 1;
}
```

There is one caveat in the code: when we find two overlapping ranges, we must replace them with their union. This essentially means that we must remove both old ranges in the correct order: first the latter, then the former. Then we insert the union back. These operations add an extra logarithmic factor.

**Complexity estimation**

Each pass over the set takes *O*(*N*²) for the pairwise comparison, plus *O*(log *N*) for the set operations. Since each pass performs exactly one merge, the number of passes is at most *N* − 1. The overall complexity is *O*(*N*³ × log *N*). For the given input, *N* = 187, which is small enough for the brute-force approach to finish quickly.

The moral: use the right data structure for the job.

[<< To Day 4](day-04.md)   [To Day 6 >>](day-06.md)
