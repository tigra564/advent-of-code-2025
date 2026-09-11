#!/usr/bin/python3

import sys
import re
import matplotlib.pyplot as plt

if len(sys.argv) < 2:
	print(f"Usage: {sys.argv[0]} <input file> [<answer file>]")
	exit(1)

points = []
with open(sys.argv[1], 'r') as infile:
	for line in infile:
		m = re.match(r'(\d+),(\d+)', line)
		points.append([int(m[1]), int(m[2])])

boxx = []
boxy = []
if len(sys.argv) > 2:
	with open(sys.argv[2]) as resfile:
		line = resfile.readline()
		nums = re.findall(r'\d+', line)
		nums = [int(n) for n in nums]

		boxpoints = [
			(nums[0], nums[1]),
			(nums[0], nums[3]),
			(nums[2], nums[3]),
			(nums[2], nums[1]),
			(nums[0], nums[1]),
		]
		#print(boxpoints)
		boxx = [p[0] for p in boxpoints]
		boxy = [p[1] for p in boxpoints]

# Close the contour by adding the first point to the end
closed = points + [points[0]]

# Split in X and Y
x = [p[0] for p in closed]
y = [p[1] for p in closed]

# Draw
plt.figure(figsize=(6, 6))
plt.plot(x, y, 'b-', linewidth=2)  # blue line for tiles contour
plt.plot(boxx, boxy, 'r-', linewidth=1) # red lines for the winning triangle

# Invert Y axis so it grows downwards
plt.gca().invert_yaxis()

plt.grid(True, linestyle='--', alpha=0.6)
plt.title("Movie theater floor tiles contour")
plt.axis('equal')  # set aspect ratio to 1
plt.show()
