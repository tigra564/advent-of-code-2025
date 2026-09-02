# Day 2: Gift Shop

## Part 1

This problem is not about numbers, but rather about strings. In the first part, we just need to verify that a number being tested has the form of "AA", where "A" is an arbitrary string of digits.

So we check if a number has an even nuber of digits and, if so, whether the first half equals the second.

## Part 2

This time, we are asked whether a given number (i.e., a string)  has the form "A{2,}" in terms of [regular expressions](https://en.wikipedia.org/wiki/Regular_expression)—meaning that any sequence of the form "AA", "AAA", "AAAA", ..., "AAA...AAA" fits, where in the last case "A" represents one digit.

We iterate over all divisors *k* of the string length *N*, from 1 up to *N-1*. For instance, if the number has 6 digits, we check lengths 1, 2, and 3 (but not 6). For each divisor *k*, we take the first *k* characters, repeat them *N/k* times, and check if the resulting string matches the original. If so, we've found another invalid ID. We store it in a result vector and finally sum up the accumulated values.

[<< To Day 1](day-01.md)   [To Day 3 >>](day-03.md)
