# Day 8: Playground

# Part 1

We start by parsing the jbox coordinates and storing them in a vector as 3-integer tuples. Then we calculate distances (actually their squares, to avoid taking square roots) for every pair of jboxes. A distance structure stores the identifiers of the jbox coordinates (their offsets in the vector) and the distance value. For brevity, let's call the jbox coordinates identifiers simply "jbox ids".

After that, we take the smallest *N* distances (*N* = 10 for the sample input, and 1000 for the puzzle input) and start forming clusters of jboxes.

Clusters are stored in a hash map. Its keys are integer cluster identifiers, and each value is a vector of jbox ids and an identifier of the cluster it was merged with. When two clusters merge, the jbox ids are moved from the cluster with the larger id to the cluster with the smaller id. The cluster with the larger id stores the id of the other cluster, since its data was moved there.

If either of these two clusters is marked as moved, we traverse the chain of movements until we reach a cluster that hasn't been moved yet, and reset the initial cluster's moved-to identifier accordingly. This prevents us from traversing long chains more than once.

The clustering process stops when some cluster's vector of jbox ids contains all jbox ids (i.e., its size equals the total number of jbox coordinates).

Initially, the clusters vector is populated with single‑jbox clusters—i.e., the number of clusters equals the number of jboxes, and the set of cluster ids matches the set of jbox ids. The moved-to property of each cluster is set to -1, indicating that its jbox ids haven't been moved yet.

After the clusters have been formed, we choose the 3 largest and calculate the result.

## Part 2

Most of the algorithmic code for this part is shared with Part 1. The solution for this part breaks into:

* parsing the jbox coordinates
* calculating the squared distances and sorting them in ascending order
* forming clusters in the same manner as in Part 1
* calculating the result

[<< To Day 7](day-07.md)   [To Day 9 >>](day-09.md)
