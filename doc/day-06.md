# Day 6: Trash Compactor

## Part 1

The problem is essentially about how and into what we read the input data. In Part 1, we just read the first record, figure out the number of math exercises, and then proceed with the main job. We don't know the number of lines in the homework, so we must decide when the operands end and the operator appears—much like in [reverse Polish notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation). We do this by inspecting the first symbol of each field in the line.

Each line is parsed by `operator>>()`, which throws away all whitespace characters. This makes reading separate numbers and operations straightforward.

We have two vectors: `numbers` for storing operands and `ops` for storing operations (`std::multiplies` or `std::plus`). After reading input, we just apply `ops` to `numbers`.

```c++
switch (sym) {
case '*':
	ops.emplace_back(i64{1}, std::multiplies<i64>());
	break;
case '+':
	ops.emplace_back(i64{0}, std::plus<i64>());
	break;
default:
	numbers[i].emplace_back(std::stoll(field));
}
```

In Part 1, the operations are stored as `std::function`. This is flexible but not free: the call goes through an indirect dispatch, and the compiler usually cannot inline it. For two operations and a few thousand calls, the overhead is negligible. If this were a hot path with millions of calls, an `enum` with a `switch` would be a better choice. The same principle applies here as everywhere else in this repository: measure before optimizing.

**Complexity estimation**

The input is a grid of *R* lines and *C* columns. We read each line once and parse it field by field, so the complexity is *O*(*R* × *C*).

## Part 2

Though the input remains the same, this time we cannot exploit `operator>>()` to read the data, because whitespace characters affect the positioning of the numbers.

We store the homework in a vector of strings. The cephalopods read data right-to-left and downwards, so do we. The outer loop scans from right to left. The inner loop scans down to the end, but not including the operators from the last line. For each block of numbers, the operator appears in the leftmost column of the block. This allows us to first gather the numbers into a vector, then apply the operation, add the result to the total, and discard the vector.

```c++
for (int i = syms[0].size() - 1; i >= 0; i--) {
	bool is_empty = true;
	auto number = i64{0};
	for (std::size_t j = 0; j < syms.size() - 1; j++) {
		auto sym = syms[j][i];
		if (' ' == sym)
			continue;
		is_empty = false;
		number = number * 10 + (sym - '0');
	}
	if (!is_empty)
		numbers.emplace_back(number);
```

Note that the outer loop uses `int`, not `std::size_t`. This is deliberate: the loop runs backwards and checks `i >= 0`. With `std::size_t`, the condition would always be true, and the loop would never terminate. This is the same class of bug as `for (unsigned int i = size(s) - 1; i >= 0; i--)`, which is a classic source of infinite loops.

```c++
i64 partial;
switch (op) {
default: continue;
case '*':
	partial = std::accumulate(numbers.begin(), numbers.end(),
		i64{1}, std::multiplies<i64>());
	break;
case '+':
	partial = std::accumulate(numbers.begin(), numbers.end(),
		i64{0}, std::plus<i64>());
	break;
}
```

**Complexity estimation**

The outer loop runs over *C* columns, the inner loop over *R* lines. Each cell is visited exactly once, so the complexity is *O*(*R* × *C*), same as in Part 1.

[<< To Day 5](day-05.md)   [To Day 7 >>](day-07.md)
