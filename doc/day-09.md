# Day 9: Movie Theater

## Part 1

Here we just loop through every pair of tiles and calculate the maximum area of the rectangles they form.

## Part 2

### Obvious solution

With the [Boost Geometry library](https://www.boost.org/doc/libs/latest/libs/geometry/doc/html/index.html), this part is nearly as simple as Part 1. Indeed, we again iterate over all pairs of tiles, select only those that form a rectangle fully covered by the polygon formed by the red and green tiles, and return the maximum area among the valid rectangles.

The task was trivial and boring, so I asked an AI to generate the code for this part. After three iterations, it produced [`09-2-boost.cpp`](../09-2-boost.cpp), which gives the correct answer. The code could have been more concise if the AI hadn't run into integer overflow. I should have told it to use `unsigned long long` beforehand.

However, I made this AI‑generated solution after I had already crafted a hand‑made one. Using `covered_by` from Boost.Geometry was the obvious approach, but I wanted to see if I could make through without external libraries.

### First bruises

Without Boost Geometry, the puzzle turns into a not-so-trivial problem. However, there was still hope.

Here is a picture of the colored contour for the sample input.

```
..............
.......#XXX#..
.......X...X..
..#XXXX#...X..
..X........X..
..#XXXXXX#.X..
.........X.X..
.........#X#..
..............
```

It looks "convex". A convex figure contains all points of the straight lines segments between any two points belonging to it. If we restrict the term "straight line" to only the allowed movements—i.e., vertical or horizontal—then under this restriction, the sample could be considered an HV-convex polyomino (see section 3 of [this](https://ar5iv.labs.arxiv.org/html/2004.07354) article). Since the figure enclosed by the input line is a polyomino by the puzzle statement, further we will refer to such a property simply as HV-convexity.

If the main input were HV-convex, too, the task would be tons easier. Indeed, we could simply check whether all four vertices of a rectangle lie inside the contour; that would be enough to deduce that the whole rectangle also lies inside. Two of those points by definition belong to the contour. We must check only the remaining two.

Cchecking for a single point *P* is quite easy. It's enough to find a couple of, say, vertical segments *V1* and *V2* with the following properties:

* *P*'s Y‑coordinate lies within the intervals of the end Y‑coordinates of both segments.
* *P*'s X‑coordinate lies within the interval between *V1*'s X‑coordinate and *V2*'s X‑coordinate.

That's it! We could even sort the segments by their upper ends to speed up the search a little.

So I tried to check the main input for HV-convexity. I wrote a script [`09-check-hvconvexity.py`](../09-check-hvconvexity.py) that takes an input file as its argument. The check for HV-convexity is simple. At each step, the path turns left or right. To tell left from right, we take two segments forming a turn and compute their cross product. If the product's sign is negative, it's a right turn (remember, the Y-coordinate grows downward); otherwise it's a left turn.

We compute the difference between the numbers of left and right turns. It must be exactly 4 for a closed contour, since we end up at the starting point after a full 360-degree rotation (either left or right). Additionally, if our HV-convex path is right-oriented, it is allowed to have no more than one consecutive left turn and no more than four consecutive right turns. The same applies to a left-oriented HV-convex figure, but with left and right swapped.

Alas, the main input turned out not to be HV‑convex. This bad news led me later to a serious mistake.

### Drawing a circular path

We can still cope with a non-HV-convex contour by applying the following strategy. First, we check that no segment of the contour crosses the interior (excluding the borders) of a rectangle being tested. Then we may choose a point strictly inside the rectangle and trace the entire contour, measuring the total angle swept around the point. If the point is inside the contour formed by the path, the whole rectangle is valid. A point is inside the contour if, while traversing the contour, we make a full circle around it.

We start at the beginning of the path, measure the angle between the first red tile, the point of interest, and the second point. We make one step along the path, measure the next angle, and add it to the previous one. In the end, we will either get a 360-degree total angle, which means a full circle, or 0 degrees, meaning the contour somehow went around the point without enclosing it.

It's a valid solution, but it looked too cumbersome to me for a one-day puzzle, so I gave up on it and returned to the puzzle statement.

### Over-complication out of the blue

I carefully reread the puzzle statement twice. It said that the only guarantee is that the path always turns left or right at each red tile. Nothing is said about the number of turns. Moreover, nothing is said about how far apart the segments may lie. For instance, here is an absolutely legit input:

```
.#XXXX#
.X....X
.X.#XX#
.X.#XX#
.X....#
.#XXXX#
```

When we flood-fill it with green, the result is:

```
.#XXXX#
.XXXXXX
.XX#XX#
.XX#XX#
.XXXXX#
.#XXXX#
```

The whole big rectangle is a valid solution because those two middle segments are vertically adjacent. This means that under certain conditions, a valid rectangle may cross the contour. However, the puzzle author didn't mean that at all!

### Drawing pictures

It's the first (and the last for the 2025 event) time I resorted to visualizing the input data. You can use script [`09-vis.py`](../09-vis.py) for this. Its first argument is a puzzle input. The second optional argument is a file containing the rectangle (the answer) containing four coordinates: bottom, left, top, right. The input contour is drawn in blue, and the answer is drawn in red.

The picture gave a gloomy prospect. Here it is.

![Full contour of tiles](day-09-tiles-all.png)

Let's zoom in on it near the middle of the right border.

![Zoomed](day-09-tiles-zoomed.png)

Compare it to the sample input.

![Sample](day-09-sample.png)

The figure in the main input has plenty of cavities, and additionally a big one in the center. Things started looking dark.

### Calling for help from brute force

Let's try the other extreme and just flood fill the whole contour tile by tile. However there are two aspects which, combined, render this approach barely fruitful.

First, the input figure is about 100,000 by 100,000, which gives about 10 billion tiles. If every tile is encoded as a `char`, the whole field would require 9+ GiB of memory, and who knows how quickly we could check every rectangle against this amount of data. Even encoding a tile as a single bit would reduce the memory consumption to no less than 1 GiB. That's not much help.

The second aspect is much worse. It turns out that the problem cannot be solved by a one-dimensional algorithm. For instance, consider some row in the middle of the grid. It might look like this:

```
..#???#?????#???#.
```

Here, the question marks denote non-red tiles of interest. Should we paint them green or, say, leave them "gray"? There is no answer until we know what lies right above this row. It could be this:

```
..XXXXX.....XXXXX.    ..XXXXX.....XXXXX.
..#???#?????#???#. -> ..#XXX#.....#XXX#.
..................    ..................
```

or this:

```
..XXXXXXXXXXXXXXX.    ..XXXXXXXXXXXXXXX.
..#???#?????#???#. -> ..#XXX#XXXXX#XXX#.
......XXXXXXX.....    ......XXXXXXX.....
```

### Turning to 2-D

If the problem cannot be solved in 1-D, we can rethink it as a problem of splitting the whole field into a set of non-intersecting rectangles, some of them filled in green (and a bit red) and others staying "gray".

We will need only the set of horizontal segments of our contour, since the vertical ones can be easily restored from them. We start with a singular empty rectangle spanning the whole world with coordinates `{left: -Infinity, right: +Infinity, bottom: -Infinity, top: +Infinity}`. This time, we don't bother with the direction of the Y-axis and dуашту the bottom as the horizontal side with the minimal Y-coordinate.

We go from bottom to top and pick the closest horizontal segment. If a segment intersects a rectangle, it divides that rectangle into at most four rectangles: left, right, fixed, and new. Here is a rough picture.

```
llllffffffffffrrrrr  
llllffffffffffrrrrr  
llllffffffffffrrrrr  
llllffffffffffrrrrr  
llllffffffffffrrrrr  
llllffffffffffrrrrr  
llll##########rrrrr  
llllnnnnnnnnnnrrrrr  
llllnnnnnnnnnnrrrrr  
llllnnnnnnnnnnrrrrr  
llllnnnnnnnnnnrrrrr 
```

Here, the segment is denoted by a line of '#' symbols. The picture is rough because we'll introduce stricter rules on forming rectangles a bit later.

Left and right rectangles continue growing vertically to +Infinity. The fixed rectangle is, well, fixed because its left, right, bottom, and top borders are now defined. The new rectangle is somewhat the continuation of the fixed one and has its top border at +Infinity.

Let's introduce some notation. The rectangle being split has borders *L, R, B, T*. Since we go bottom-up, the *T* border is always at +Infinity. The splitting segment has coordinates *l, r*, and *y* (its Y-coordinate).

Here are the rules for splitting and merging rectangles. The rules are not very complicated. They emerge from the fact that if a vertical or horizontal segment splits a filled and an empty rectangle, the part of the segment belongs to the filled rectangle.

1. If the rectangle is filled and has only one common point with the segment, either on the left or on the right border, then it is actually a border junction and there is no split.
2. If the rectangle being split is empty, then:
   * If *l > L*, then we have a left rectangle with borders: *L, l - 1, B, T*.
   * If *r < R*, then we have a right rectangle with borders: *r + 1, R, B, T*.
   * The rectangle being fixed fits horizontally between the left and right rectangles (if they exist), or spans to the left or right border (or both) of the initial rectangle. Its bottom border is *B*, and its top border is *y - 1*. If *y < B*, the fixed rectangle does not exist. This handles the case when two segments are vertically adjacent.
   * Similarly, the new rectangle fits horizontally between the left and right and gets bottom border *y* and top border *T*.
   * The left, right, and fixed rectangles are set as empty, while the new rectangle is filled.
3. If the rectangle being split is filled, then:
   * If *l >= L*, then we have a left rectangle with borders: *L, l, B, T*.
   * If *r <= R*, then we have a right rectangle with borders: *r, R, B, T*.
   * The rectangle being fixed fits horizontally between the left and right rectangles (if they exist), or spans to the left or right border (or both) of the initial rectangle. Its bottom border is *B* and its top border is *y*.
   * Similarly, the new rectangle fits horizontally between the left and right and gets bottom border *y + 1* and top border *T*.
   * The left, right, and fixed rectangles are set as filled, while the new rectangle is empty.
4. If a segment splits several rectangles in a row, the resulting new rectangles are merged into one, with the left being the leftmost of the set, the right border the rightmost, and obviously, the bottom and top borders are common to all of them.

As mentioned earlier, we start with one empty rectangle containing the whole plane. We end up with a set of fixed rectangles and some leftover rectangles that were not split by horizontal segments. We add these to the set of fixed ones.

That's all.

Now we just iterate over all rectangles to be tested for coverage. It's enough to find at least one empty rectangle that shares at least one tile with the rectangle under test to conclude that the tested rectangle is not fully covered by the red and green tiles.

### Post-credits scene

The algorithm gives the same result for the given sample input and the main input. However, the file [`09.txt.tricky-sample`](../09.txt.tricky-sample) shows the difference.

Here is the answer given by the Boost implementation.

![](day-09-tricky-sample-boost.png)

And here's the result of the hand-crafted implementation.

![](day-09-tricky-sample.png)

The blue segments intersecting the red rectangle are in fact vertically adjacent and result in no gap. The algorithm also deals correctly with horizontally adjacent segments because it will not put any empty rectangle between them.

[<< To Day 8](day-08.md)   [To Day 10 >>](day-10.md)
