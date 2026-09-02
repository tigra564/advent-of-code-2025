# Day 6: Trash Compactor

## Part 1

The problem is essentially about how and into what we read the input data. In Part 1, we just read the first record, figure out the number of math exercises, and then proceed with the main job. We don't know the number of lines in the homework, so we must decide when the operands end and the operator appears — much like in [reverse Polish notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation). We do this by inspecting the first symbol of each field in the line.

Each line is parsed by `operator>>()`, which throws away all whitespace characters. This makes reading separate numbers and operations easy.

We have two vectors: `numbers` for storing operands and `ops` for storing pointers to operations (`std::multiplies` or `std::plus`). After reading input, we just apply `ops` to `numbers`.

## Part 2

Though the input remains the same, this time we cannot exploit `operator>>()` to read the data, because whitespace characters affect the positioning of the numbers.

We store the homework in a vector of strings. The cephalopods read data right-to-left and downwards, so will we. The outer loop scans from right to left. The inner loop scans down to the end, but not including the operators from the last line. For each block of numbers, the operator appears in the leftmost column of the block. This allows us to first gather the numbers into a vector, then apply the operation, add the result to the total, and discard the vector.

[<< To Day 5](day-05.md)   [To Day 7 >>](day-07.md)
