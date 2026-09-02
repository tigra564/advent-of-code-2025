# Day 12: Christmas Tree Farm

This day has only one part, but at first glance it looks very scary.

The puzzle asks us to solve a packing problem for non‑convex figures. Generally, this is an NP‑complete problem and, obviously, cannot be solved in a single day — especially on Christmas Eve.

A simple solution is sufficient. We can just substitute each figure with its bounding box. There is no need to even parse the figures, since all of them are 3×3 cells. The problem then reduces to whether such a box can fit into the given geometries. To determine this, we take the integer part of each dimension divided by the box size (i.e., 3), multiply these values, and compare the product to the total number of figures that need to be placed.

And that's all there is to it!

[<< To Day 11](day-11.md)
