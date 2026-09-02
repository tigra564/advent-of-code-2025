# Day 3: Lobby

## Part 1

The maximum joltage is given by the highest digit in the whole data string, excluding the last element—because if we find, say, '9' in the last position, we don't haveany digits left to form the second digit of the resulting number.

Thus, we seek for the maximum digit in the string excluding the last element, select it, and then look for the maximum digit in the remaining tail (including the last digit).

## Part 2

It's exactly the same, but for a larger number of digits to collect. The algorithm is the same: at each iteration, we look for the maximum digit in the remaining string, but we must not touch the `i` rightmost digits, since we need to leave room for the rest of the resulting number. Each time we find a new maximum digit, we multiply the joltage accumulated so far by 10 and add the value of the newly found digit.

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

This turned out to be the easiest part 2 compared to part 1 in the whole event.

[<< To Day 2](day-02.md)   [To Day 4 >>](day-04.md)
