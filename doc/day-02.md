# Day 2: Gift Shop

## Part 1

Despite the numeric input, this problem is really about strings. In the first part, we just need to verify that a number being tested has the form of "AA", where "A" is an arbitrary string of digits.

```c++
if (sz % 2)
	continue;

auto hi = s.substr(0, sz / 2);
auto lo = s.substr(sz / 2);

if (hi == lo)
	inv.emplace_back(i);
```

So we check if a number has an even number of digits and, if so, whether the first half equals the second. If both conditions hold, the number is invalid.

**Complexity estimation**

The algorithm is *O*(*N* × *d*), where *N* is the number of values in the range and *d* is the average number of digits. For the given input, this is fast enough.

## Part 2

This time, we are asked whether a given number (i.e., a string)  has the form "(A){2,}" in terms of [regular expressions](https://en.wikipedia.org/wiki/Regular_expression)—meaning that any sequence of the form "AA", "AAA", "AAAA", ..., "AAA...AAA" fits, where "A" is a string of one or more digits.

Part 2 generalises Part 1: instead of two halves, we allow any number of repetitions.

We iterate over all *k* from 1 to *N*-1 and check whether *k* divides *N*. For instance, if the number has 6 digits, we check lengths 1, 2, and 3 (but not 6). For each divisor *k*, we take the first *k* characters, repeat them *N/k* times, and check if the resulting string matches the original. If so, we've found another invalid ID.

```c++
for (std::size_t k = 1; k < sz; k++) {
	if (sz % k)
		continue;

	auto patt = s.substr(0, k);
	std::string candidate;
	candidate.reserve(sz);
	for (std::size_t p = 0; p < sz / k; p++) {
		candidate += patt;
	}

	if (candidate == s) {
		inv.emplace_back(i);
		break;
	}
}
```

Note the `break`. Once we have found that the ID can be represented as a repetition of some pattern, it is invalid. We do not need to check the remaining divisors. This also means we do not need a `std::set` for deduplication: each ID is added to the result at most once.

The types of `k` and `p` are `std::size_t` because they are compared with `sz`, which is also `std::size_t`. Mixing signed and unsigned types in comparisons is a common source of bugs.

We store it in a vector and sum the values at the end.

**Complexity estimation**

The algorithm is *O*(*N* × *d*²), where *N* is the number of values and *d* is the number of digits. The inner loop runs over all divisors of *d*, and for each divisor we build and compare a string of length *d*.

**Alternative approach**

An alternative would be to generate invalid IDs directly, by iterating over possible pattern lengths and repeating the pattern. This would be faster in theory, but it is more complex: it requires handling numbers of different lengths, patterns that start with zero, and duplicates (a number like 1111 can be generated in more than one way). For the size of the AoC input, the direct approach is fast enough, and it is simpler to reason about.

[<< To Day 1](day-01.md)   [To Day 3 >>](day-03.md)
