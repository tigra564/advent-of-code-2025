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

## Part 2

This puzzle is a slight evolution of Part 1. We just do what is written in the puzzle statement verbatim.

* Set up a counter for removed rolls to 0.
* Scan every occupied cell (marked with '@').
* Count its neighbors.
* If the number is less than four, mark the cell with 'X' and increment the counter of removed rolls.
* In the end, re-mark all 'X' with '.' since we actually remove them.
* Add the counter value to the total result.
* If the counter is not zero, repeat the procedure.

[<< To Day 3](day-03.md)   [To Day 5 >>](day-05.md)
