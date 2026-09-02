# Advent of Code 2025 solutions

## The Goal

This repository contains solutions for [Advent of Code 2025](https://adventofcode.com/2025). The code is written in C++ using only what is provided by the compiler and the standard library. The goal was not to achieve maximum performance or fast delivery, but rather to prove whether it is possible to solve all the problems in a reasonable time without specialized tools.

In fact, everything went smoothly and quickly until Part 2 of Day 9, which took me several evenings to solve. Part 2 of Day 10 took me one more evening than Part 2 of Day 9, which I spent studying integral linear programming techniques.

The code is written in a subset of C++ 17 supported by GCC v12.5.0. There are some alternative solutions that show how much easier, faster and generally more productive it is to use tools specifically designed for the problem at hand. These use Boost v1.74.0. That was the version available on my laptop when I created this repository. I didn't use any external linear programming libraries, so as not to bother those who might try to compile the code with additional dependencies. That's why there is no alternative solution for Part 2 of Day 10.

I started with quick-and-dirty approach to solving part 1, then I created the initial code for part 2 by copy-pasting. However, after getting all 24 stars, I took my time to polish the solutions a bit. Hence, files like `common.hpp` (used for all days) and `08-common.hpp` (specifically for Day 8) appeared.

## Building and running

Inside this directory:

```sh
mkdir build && cd build
cmake ..
make -j 24
```

Each part of a day is solved by a separate binary, which takes a data file as its command-line argument. You can toss it your personal input file for the puzzle or use one of the samples provided in the repository.

```sh
./09-2 ../09.txt.sample
```

or

```sh
./09-2 ../09.txt
```

## Links to documentation for daily solutions

[Day 1](doc/day-01.md)
[Day 2](doc/day-02.md)
[Day 3](doc/day-03.md)
[Day 4](doc/day-04.md)
[Day 5](doc/day-05.md)
[Day 6](doc/day-06.md)
[Day 7](doc/day-07.md)
[Day 8](doc/day-08.md)
[Day 9](doc/day-09.md)
[Day 10](doc/day-10.md)
[Day 11](doc/day-11.md)
[Day 12](doc/day-12.md)
