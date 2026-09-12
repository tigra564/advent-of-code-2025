# Day 4: Printing Department

## Part 1

The task is easy. We just scan through every cell in the grid occupied by a roll of paper and count its neighbors. But when we are on the border of the grid, say, the left one, there are no neighbors to its left.

We can simply add some empty space around the borders to address this issue. This trick does not change the puzzle, but it makes coding easier. Here is a picture.

```
                ............
..@@.@@@@.      ...@@.@@@@..
@@@.@.@.@@      .@@@.@.@.@@.
@@@@@.@.@@      .@@@@@.@.@@.
@.@@@@..@.      .@.@@@@..@..
@@.@@@@.@@      .@@.@@@@.@@.
.@@@@@@@.@  ->  ..@@@@@@@.@.
.@.@.@.@@@      ..@.@.@.@@@.
@.@@@.@@@@      .@.@@@.@@@@.
.@@@@@@@@.      ..@@@@@@@@..
@.@.@@@.@.      .@.@.@@@.@..
                ............
```

The dots are not rolls of paper, so they do not affect the neighbor count. At the same time, the indices `i-1`, `j-1`, `i+1`, `j+1` are always within the grid, and we do not need to check the borders separately.

```c++
auto num = std::count_if(directions.begin(), directions.end(),
	[&grid, i, j](const auto& dir) -> bool {
		return grid[i + dir.first][j + dir.second] == '@';
	}
);

if (num < threshold)
	result++;
```

**Complexity estimation**

For each cell of the grid, we check its eight neighbours. The complexity is *O*(*R* × *C*), where *R* and *C* are the grid dimensions.

## Part 2

Part 2 is a small extension of Part 1. We follow the puzzle statement verbatim.

* Set up a counter for removed rolls to 0.
* Scan every occupied cell (marked with '@').
* Count its neighbors.
* If the number is less than four, mark the cell with 'X' and increment the counter of removed rolls.
* In the end, replace all 'X' with '.' because they are removed from the grid.
* Add the counter value to the total result.
* If the counter is not zero, repeat the procedure.

In the code, the threshold is a constant: `constexpr int threshold = 4;`.

```c++
if (num < threshold) {
	count++;
	grid[i][j] = 'X';
}
```

Marking a cell with 'X' instead of '.' is deliberate. During one round, a removed roll still occupies its cell, so it counts as a neighbour for other rolls in the same round. Only at the end of the round do we replace all 'X' with '.', so that in the next round the removed rolls no longer affect the count.

```c++
for (std::size_t i = 1; i < grid.size() - 1; i++) {
	for (std::size_t j = 1; j < grid[i].size() - 1; j++) {
		if (grid[i][j] == 'X')
			grid[i][j] = '.';
	}
}
```

The `main` function is a simple loop. `remove` returns the number of rolls removed in one round. The loop continues while that number is not zero.

```c++
while (auto num = remove(grid)) {
	result += num;
}
```

**Complexity estimation**

Each call to `remove` scans the whole grid, which takes *O*(*R* × *C*). The number of calls is the number of removal rounds.

There is an intuitive argument that bounds the number of rounds. A cell is removed when it has fewer than four neighbours. The neighbours of a cell lie on the lines `y = ±x`, `y = 0`, `x = 0` relative to it. In each round, every isolated structure is eroded by at least one full layer along at least one of these lines. If a round removes nothing from a structure, that structure is stable and will not be touched again. The maximum extent of any structure in these directions is *O*(max(*R*, *C*)), so the number of rounds is *K* = *O*(max(*R*, *C*)). The overall complexity is *O*(max(*R*, *C*) × *R* × *C*).

In practice, the number of rounds is much smaller. For the given input, it is 31, and the process removes 969 rolls. The convergence is slow only at the end, when a single long-lived island remains, but even it stabilises after a few more rounds.

Here is the initial grid.


```
...............................................................................
.@.@@@@.@@@@..@@.@....@@@.@@..@@@@....@..@@@..@.@..@@.@@@@..@.@@@@@..@@@.@..@..
.@.@@@.@.@..@@@@@@@@@@@@@@.@@@...@.@@@...@@@@@.@@@.@.@@..@@@.@..@.@@@..@@@...@.
.@.@@@...@@.@.@@@.@@@@..@@.@@@@.@@@@......@..@@.@@@@@@@..@.@@@@@@@..@@.@.@.@@@.
..@.@.@.@.@@@...@@@@@@.@@@@.@@@@@@@..@@@..@@@@@@.@@@@@@@@.@....@@.@@@@.@@.@@@@.
.@@..@..@@.@@@...@@@@.@...@.@@@@@..@...@.@...@.@@@..@@@...@..@@.@@@@@@.@..@@@@.
.@@@.@@@@@.@@@.@@@.@@.....@.@@..@..@..@@@@.@.@@@.@.@.@@@@@@@.@@@...@@@@@@@..@@.
.@.@.@@..@@@@@@@@.@.@@.@@..@@@.@@@@@@.@.@@..@@@.@@.@@@@@@@@@.@@@@..@@@@@@@@.@@.
.@@@@.@@...@@@.@@.@@@@.@@@..@@@@@@@@...@.@@@@@@@@@...@@@@@@@@..@..@@.@.@@@.....
.@.@@@..@@@@...@@..@..@@@...@@.@@.@@..@.@.@@...@.@@@@.@..@@@@@@.@.@.@@@@..@@@@.
.@@@@@@@@..@@@@@@..@@.@@@@...@@@@.@.@.@@@@.@@.@@..@@@@...@.@.@@@.@@@@...@.@@...
.@.@@.@@.@.@@..@@@...@..@.@@@...@..@@..@@.@..@@@@@@@.@@@@@@@@@@@@.@@@.@..@.@@@.
.@.@@......@@@@@@.@...@@@..@..@@@.@....@@@.@@@@@..@@@@@.@.@@.@...@@@.@@@@@@@@@.
.@@@.@@.@@@@@@@@@@.@@@.@@@..@@@@....@..@@.@..@@...@..@...@@@@..@@@..@.@.@..@.@.
....@..@@.@.@@@@@.@@@@@.@@.@@@.@..@..@@....@@@..@.@.@....@@@@@.@@@@..@@@@@@@@@.
...@.@@@.@@@@@@@@.@.@.@@@@@@.@@.@..@@@@@.@@.@@.@@.....@@@@@@@.@.@@.@.@@@@@@@@@.
.@@..@@..@@@@@@.@@@@..@@.@@..@@@@@@@@....@.@..@@@@@@@@@.@@@.@.@@@..@...@.@.@@@.
..@@@@@@@@.@@.@..@@.@.@@.@@@@@@@@.@.@@@..@@@@@@.@@@.@.@@@..@@@.@..@@...@@@.....
.@@....@@@@@.@@@@.@@@@@@@@@.@@.@@@@.@@....@@....@.@@@.@@@.@..@@.@..@.@@@@.@.@..
..@@@..@@.@..@@@.@@@..@@@@.@..@@@.@.@@@..@..@@..@@.@@@@@@@.@.@@.@@.@....@@..@@.
..@.@.@@.@@@@@@@@....@@@..@@@.@...@.@..@@@.@@@..@@@@@@@@@@..@@@.....@.@@@@@@@@.
...@@.@@@@@@...@..@@@.@.@@..@@@@@.@@@@.@@.@@@@@@@@@@@@@@.@@@.@@@@@.@@...@....@.
...@@...@@..@.@@@...@.@..@.@@@@@@@@@@@@@@@@@@.@.@@.@@@@@.@.@@..@.@@@@.@..@@.@@.
..@.@@.@@@@.@@@.@@@@..@.@...@@@@@@@.@....@.@.@@@.@.@@@.@...@@@@.....@.@@.@@@@..
.@@@@@@@@@@...@.@@.@.@@@.@@@.@@@@@@.@@@@....@.@@@@@@@@..@@@@@.@@.@@@@@@.@@.@@@.
...............................................................................
```

And here is the result after the final removal.

```
.................................................................................
.................................................................................
.................................................................................
....................@@...........................................................
...................@@@@..........................................................
..................@@@@@..........................................................
..................@@@@...........................................................
.............@@.@@@.......................................@@.....................
............@@@@@@.......................................@@@@....................
............@@@.@@.......................................@@@@@...................
............@...@@........................................@@@@@..................
............@@..@@..........................................@.@@.................
............@@..@@.........................................@@@@@.................
............@@@@@@.........................................@@.@..................
............@@@@@@@.@@@...................................@@@@...................
.............@@@@@.@@@@@.@@...............................@@@@...................
.............@@@@@.@.@.@@@@@.............................@@@@....................
..............@@.@@@@..@@.@@..@@@........................@@@.....................
..................@@...@@.@@@@@@@@......................@@.......................
.......................@@@@@.@@.@@.....................@@@.......................
........................@@@....@@...................@@@@@@.......................
...............................@...................@@@@@@@.......................
..............................@@@@.................@@@@@@........................
.............................@@@@@@.................@@@@.........................
.............................@@@@@@..................@@..........................
..............................@@@@...............................................
.................................................................................
.................................................................................
```

[<< To Day 3](day-03.md)   [To Day 5 >>](day-05.md)
