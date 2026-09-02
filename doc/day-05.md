# Day 5: Cafeteria

## Part 1

This part is trivial. We read in all the ranges and then, for each supplied ID, check if it is within the boundaries of any of those ranges. The ranges are closed, hence we use ≤ and ≥.

## Part 2

Here things start to get complicated. The problem itself is simply a task of merging all the supplied ranges and printing out their total power (the number of elements they contain). With `boost::icl::interval_set` (see [Boost.Icl](https://www.boost.org/doc/libs/release/libs/icl/doc/html/index.html)), this can be done in a couple of minutes. Check the code in [`05-2-boost.cpp`](../05-2-boost.cpp). But without a dedicated data structure, there is some work to do.

My input file contains only 187 ranges. Even for an interpreted language, this is not a big deal. We can just brute-force the solution by comparing every pair of ranges. This gives us *O(n²)* complexity for each pass and an overall complexity of *O(n³)* since at each pass we reduce the number of ranges by at most 1.

There is one caveat in the code: when we find two overlapping ranges, we must replace them with their union. This essentially means that we must remove both old ranges in the correct order: first the latter, then the former. Then we insert the union back. These operations add another *O(log n)* factor giving an overall complexity of *O(n³ · log n)*.

The moral: use the right tool for the job.

[<< To Day 4](day-04.md)   [To Day 6 >>](day-06.md)
