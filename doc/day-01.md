# Day 1: Secret Entrance

## Part 1

The first part is straightforward. Depending on the direction, we just add or subtract the given number of clicks.

After each turn, we check if the result is 0 modulo 100. If it is, then we increment the counter.

## Part 2

This one is trickier. First, if you have 17 on the dial and receive "L33", the dial will pass through zero once, even though the number of clicks is relatively small.

To avoid coding for such special cases, we take into account which direction we have to dial. If it's left, then we take a the -100's complement of the current dial value (-83 in our case). After that, we add the needed amount and just check how many hundreds we are away from zero. That gives us the number of passes through zero for this turn, though the value is negative, so we must take its absolute value. For example, if the dial is at 17, and the command is "L133". We compute (17-100) - 133 = -220, divide it by 100, and take the absolute value to finally get 2.

Second, to be consistent, we must set the dial to a positive position after each turn. In this example, we must convert -220 to 80. However, -220 % 100 gives us -20 (since C++ 11), because the standard says that the result must have the same size as the dividend. To get to the positive side, we do this little trick:

```c++
pos = ((pos % 100) + 100) % 100;
```

We first get the remainder, then add 100 to make sure we get a positive result, and finally apply modulo once more. This gives the required result regardless of the sign of `pos`.

[To Day 2 >>](day-02.md)
