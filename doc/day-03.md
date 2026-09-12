# Day 3: Lobby

## Part 1

The maximum joltage is given by the highest digit in the string. The first digit must leave at least one position to its right for the second digit. That is why we look for it in the range `[begin, end-1)`. Once the first digit is chosen, the second is the maximum digit in the remaining tail, which includes the last element of the string.

```c++
auto first = std::max_element(record.begin(), record.end() - 1);
auto second = std::max_element(first + 1, record.end());

result += (*first - '0') * 10 + (*second - '0');
```

We look for the maximum digit in the string, excluding the last element. We select it, and then look for the maximum digit in the remaining tail, including the last digit.

**Complexity estimation**

For each line, we scan it twice: once to find the first digit and once to find the second. The complexity is *O*(*N* × *L*), where *N* is the number of lines and *L* is the line length.

## Part 2

It is the same problem, but for a larger number of digits. Part 1 is a special case of Part 2 with two digits to collect.

The algorithm is greedy: at each step we pick the largest digit that still leaves enough room for the remaining digits. This is optimal because a larger digit in a more significant position always outweighs any possible choice later. A smaller digit now could only be compensated by a larger digit later, but the later digit occupies a less significant position.

We must leave room for the remaining `i` digits, where `i` is the loop counter running from `ndigits - 1` down to 0. In the code, the number of digits to collect is a constant: `constexpr int ndigits = 12;`.

```c++
auto start = record.begin();
for (auto i = ndigits - 1; i >= 0; i--) {
	auto digit_it = std::max_element(start, record.end() - i);
	joltage = joltage * 10 + (*digit_it - '0');
	start = digit_it + 1;
}
```

Here is a visualization for some line of input.

```
12323487731859253325149927242314362244355249

 123234877318592533251499272423143 62244355249 -> 9
              ^
12323487731859 25332514992724231436 2244355249 -> 99
                       ^
12323487731859253325149 927242314362 244355249 -> 999
                        ^
123234877318592533251499 272423143622 44355249 -> 9997
                          ^
12323487731859253325149927 24231436224 4355249 -> 99976
                                  ^
1232348773185925332514992724231436 2244 355249 -> 999764
                                     ^
1232348773185925332514992724231436224 43 55249 -> 9997644
                                      ^
12323487731859253325149927242314362244 35 5249 -> 99976445
                                        ^
1232348773185925332514992724231436224435 5 249 -> 999764455
                                         ^
12323487731859253325149927242314362244355 2 49 -> 9997644552
                                          ^
123234877318592533251499272423143622443552 4 9 -> 99976445524
                                           ^
1232348773185925332514992724231436224435524 9  -> 999764455249
                                            ^
```

Each time we find a new maximum digit, we multiply the joltage accumulated so far by 10 and add the value of the newly found digit.

This turned out to be the easiest Part 2 in the whole event.

**Complexity estimation**

For each line, we call `std::max_element` twelve times. But each call scans only the remaining part of the line. Thus we pass through each digit at most once, so for each input line we scan it till the end in the worst case, which gives the overall complexity of *O*(*N* × *L*).

[<< To Day 2](day-02.md)   [To Day 4 >>](day-04.md)
