# Day 12: Christmas Tree Farm

This day has only one part, but at first glance it looks very scary.

The puzzle asks us to solve a packing problem for non-convex figures. Generally, this is an NP-complete problem and, obviously, cannot be solved in a single day—especially on Christmas Eve. Everyone should be preparing for the holiday, not sitting over a potentially NP-complete problem.

Thankfully, we do not have to solve it in general. In my input, all the shapes are 3×3 squares. So we can substitute each figure with its bounding box, which is also 3×3. There is no need to even parse the shapes themselves.

The problem then reduces to a simple counting exercise: for each region of size *W* × *H*, how many 3×3 boxes can fit into it? Since the boxes are axis-aligned and identical, the answer is `(W / 3) * (H / 3)`. We compare this maximum to the total number of shapes that need to be placed, and if the shapes fit, we count the region as solvable.

```c++
// Just ignore first 30 lines which hold gift shapes
constexpr auto lines_to_skip = 30;
for (int i = 0; i < lines_to_skip; i++) {
	std::getline(file, record);
}

auto result = 0;
while (std::getline(file, record)) {
	auto x = record.find('x');
	auto c = record.find(':');
	auto w = std::stoi(record.substr(0, x));
	auto h = std::stoi(record.substr(x + 1, c - x - 1));
	auto max_blocks = (w / 3) * (h / 3);

	std::stringstream quantities(record.substr(c+1));
	auto num_blocks = 0;
	std::string quant;
	while (quantities >> quant)
		num_blocks += std::stoi(quant);

	if (num_blocks <= max_blocks)
		result++;
}
```

The first 30 lines of the input describe the gift shapes. We skip them entirely, because we do not need to know their exact form—only that they fit into a 3×3 bounding box. This is the source of the magic constant `lines_to_skip = 30`.

If the shapes were all different sizes, or worse, not square, the problem would become much harder. But it is Christmas Eve, and we just want to get the result and the star. The pair to that star will be delivered by Santa.

**Complexity estimation**

Skipping the first 30 lines takes *O*(1). For each remaining line, we parse the region dimensions and the list of shapes, which takes *O*(*L*), where *L* is the length of the line. The total complexity is *O*(*N* × *L*), where *N* is the number of regions.

For the given input, this runs in a fraction of a second.

[<< To Day 11](day-11.md)
