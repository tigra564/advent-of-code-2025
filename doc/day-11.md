# Day 11: Reactor

## Part 1

Here we implement the same approach as we used for counting paths in Part 2 of Day 7. The only slight difference is that here recursion is implemented with a lambda function, and there is no need to construct a custom hash for the key.

## Part 2

Brute‑forcing Part 2 is not an option. In my case, the result amounted to 390,108,778,818,526 (hundreds of trillions!) paths.

Also, we cannot simply copy the approach from Part 1, and here is why. In Part 1, we calculate all paths unconditionally, but in this case a valid path must contain both `fft` and `dac` nodes.

To handle this, it is enough to maintain four counters for each path, tracking which of the required nodes have been seen so far:

* `empty`—if neither `dac` nor `fft` has been seen yet
* `dac`—if at least one `dac` node has been seen, but no `fft` nodes
* `fft`—if at least one `fft` node has been seen, but no `dac` nodes
* `both`—if at least one `dac` and at least one `fft` node have been seen

This could be implemented as a [deterministic finite automaton](https://en.wikipedia.org/wiki/Deterministic_finite_automaton), but simply tracking the counters is enough.

A path is created when we encounter a 1-step connection ending with the `out` label. Initially, all its counters are set to 0, except the `empty` counter, which is set to 1.

For each node, we create a new path with zero counters and iterate over its outgoing connections.

The rules for updating the counters of a path, given the counter values of the paths that a connection points to, are as follows:

* If the current node is `dac`, then for each outgoing connection (to already computed paths):
  * the `dac` count is increased by the sum of the connected path's `dac` and `empty` counters
  * the `both` count is increased by the sum of the connected path's `both` and `fft` counters
* If the current node is `fft`, a similar update is performed:
  * the `fft` count is increased by the sum of the connected path's `fft` and `empty` counters
  * the `both` count is increased by the sum of the connected path's `both` and `dac` counters
* For any other node label, all counters are increased individually.

[<< To Day 10](day-10.md)   [To Day 12 >>](day-12.md)
