#!/usr/bin/python3

import sys
import re

if len(sys.argv) < 2:
	print(f"Usage: {sys.argv[0]} <input file>")
	exit(1)

points = []
with open(sys.argv[1], 'r') as infile:
	for line in infile:
		m = re.match(r'(\d+),(\d+)', line)
		points.append([int(m[1]), int(m[2])])

points += [points[0], points[1]]

total_turn = 0
lrpath = ""
for p in range(0, len(points)-2):
	x1 = points[p+1][0] - points[p][0]
	y1 = points[p+1][1] - points[p][1]
	x2 = points[p+2][0] - points[p+1][0]
	y2 = points[p+2][1] - points[p+1][1]
	vprod = x1*y2 - x2*y1
	turn = 1 if vprod > 0 else -1
	total_turn += turn
	lrpath += 'r' if turn > 0 else 'l'

num_right_turns = sorted(map(lambda s: len(s), re.findall("r+", (lrpath + lrpath))), reverse=True)[0]
num_left_turns  = sorted(map(lambda s: len(s), re.findall("l+", (lrpath + lrpath))), reverse=True)[0]
print(f"Total turn:      {total_turn} ({'right' if total_turn > 0 else 'left'})")
print(f"Max right turns: {num_right_turns}")
print(f"Max left turns:  {num_left_turns}")
print("Is HV-convex:    ", end='')
if total_turn > 0:
	print(num_left_turns == 1 and num_right_turns <= 4)
if total_turn < 0:
	print(num_right_turns == 1 and num_left_turns <= 4)



