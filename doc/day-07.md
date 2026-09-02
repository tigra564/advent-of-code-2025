# Day 7: Laboratories

## Part 1

At first glance, it would suffice to just count all the splitters. However, even the sample data contains one that is not hit (last row, second splitter from the right). We have no choice but to model the path of all beams. But first, let's study the sample and our input a bit (assuming the current working directory is `build/`).

```sh
grep '\^\^' ../07.txt{,.sample}
grep -P '^\^|\^$' ../07.txt{,.sample}
```

The first command checks if there are two splitters adjacent somewhere. The second checks whether any splitter is located on the border, which means it cannot generate a left or right beam.

In my case, both commands give empty output. This means there are no special cases, and every splitter simply splits the beam left and right.

The rest is simple. We create a string storing the current X‑coordinates of the beams (one character per possible horizontal position) and initialize it with a single `'|'` at the position of the `'S'` symbol in the first line of the input. Then, for each splitter we encounter, we check if there is a beam pointing to it. If the condition is true, we replace a corresponding portion of the string (which stores the current X‑coordinates of the beams) with `'| |'` and increment the total count.

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

We must trace each path separately. One way to do it is to use recursion. Again, we start at the position of the `'S'` symbol and call a function that calculates the number of paths below. In turn, for each splitter it calls itself for the left branch and the right, adds the results, and returns the sum.

For each path starting at a given row and column, we [memoize](https://en.wikipedia.org/wiki/Memoization) the result and use it in our calculations. In the example above, the position of the beam in the last row (and the number of paths emerging from it) is calculated once when we traverse there from the left splitter in the row above. When processing the path starting from the right splitter, we just retrieve the memoized value.

To implement memoization, we use a hash map that maps a pair `<row, column>` to the number of paths from that position. Since there is no standard hash function for such keys, we need to implement one ourselves. The hash function does not need to be unique for different inputs, but it helps if it is. In our case, it is enough to construct a 64-bit integer from two 32-bit integers by concatenation.

[<< To Day 6](day-06.md)   [To Day 8 >>](day-08.md)
