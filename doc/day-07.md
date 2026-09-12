# Day 7: Laboratories

## Part 1

At first glance, it would suffice to count all the splitters. However, even the sample data contains one that is not hit (last row, second splitter from the right). We have no choice but to model the path of all beams. But first, let's examine the sample and the input a bit (assuming the current working directory is `build/`).

```sh
grep '\^\^' ../07.txt{,.sample}
grep -P '^\^|\^$' ../07.txt{,.sample}
```

The first command checks if there are two splitters adjacent somewhere. If there are, a beam hitting the left one would split into two, and the right one might be missed or hit by the left beam. The second command checks whether any splitter is located on the border, which means it cannot generate a left or right beam.

In my case, both commands give empty output. This means there are no special cases, and every splitter simply splits the beam left and right.

The rest is simple. We create a string storing the current X-coordinates of the beams (one character per possible horizontal position) and initialize it with a single `'|'` at the position of the `'S'` symbol in the first line of the input. Then, for each splitter we encounter, we check if there is a beam pointing to it. If the condition is true, we replace the corresponding portion of the string with `'| |'` and increment the total count.

```c++
while (file >> record) {
	for (std::size_t i = 0; i < record.size(); i++) {
		if ('^' == record[i] && '|' == beams[i]) {
			result++;
			beams.replace(i-1, 3, "| |");
		}
	}
}
```

**Complexity estimation**

For each row, we scan all columns. The complexity is *O*(*R* × *C*), where *R* is the number of rows and *C* is the number of columns.

## Part 2

There is a quick answer to this part: 2 to the power of the result of Part 1. Alas, it is wrong. The catch is that in some cases two separate paths may merge somewhere in the middle. Here's an example.

```
.......S.......
.......|.......
.......^.......
......|.|......
......^.^......
.......|.......
```

Two paths merge when they arrive at the same cell. Without memoization, the number of paths from that cell would be recomputed for each incoming path.

We must trace each path separately. One way to do it is to use recursion. Again, we start at the position of the `'S'` symbol and call a function that calculates the number of paths below. In turn, for each splitter it calls itself for the left branch and the right, adds the results, and returns the sum.

For each path starting at a given row and column, we [memoize](https://en.wikipedia.org/wiki/Memoization) the result and use it in our calculations. In the example above, the position of the beam in the last row (and the number of paths emerging from it) is calculated once when we traverse there from the left splitter in the row above. When processing the path starting from the right splitter, we just retrieve the memoized value.

```c++
i64 num_paths_impl(const std::vector<std::string>& diagram, int row, int col, Memo& memo) noexcept
{
	Key key = {row, col};
	if (auto it = memo.find(key); it != memo.end()) {
		return it->second;
	}

	for (std::size_t i = row; i < diagram.size(); i++) {
		const auto& line = diagram[i];
		if ('^' != line[col])
			continue;

		i64 left = num_paths_impl(diagram, i, col-1, memo);
		i64 right = num_paths_impl(diagram, i, col+1, memo);

		auto result = left + right;
		memo[key] = result;
		return result;
	}

	memo[key] = 1;
	return 1;
}
```

To implement memoization, we use a hash map that maps a pair `<row, column>` to the number of paths from that position. Since there is no standard hash function for such keys, we need to implement one ourselves. The hash function does not need to be unique for different inputs, but it helps if it is. In our case, it is enough to construct a 64-bit integer from two 32-bit integers by concatenation: row in the upper 32 bits, col in the lower 32. This is collision-free as long as both coordinates fit in 32 bits.

```c++
using Key = std::pair<int, int>;
struct KeyHash {
	std::size_t operator()(const Key& k) const noexcept
	{
		return (static_cast<i64>(k.first) << 32) + k.second;
	}
};
```

**Complexity estimation**

Without memoization, the number of paths grows exponentially, and the recursive tree would have *O*(2^*R*) nodes in the worst case. With memoization, each `(row, col)` pair is computed at most once. The number of distinct pairs is *O*(*R* × *C*), and each computation scans downwards from its starting row, which takes *O*(*R*) in the worst case. The overall complexity is *O*(*R*² × *C*). For the given input, this is fast enough.

[<< To Day 6](day-06.md)   [To Day 8 >>](day-08.md)
