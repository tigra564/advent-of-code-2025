#include <array>
#include <algorithm>
#include <iterator>
#include <limits>

#include "common.hpp"

//#define AOC_DEBUG

using Tile = std::array<int, 2>;
using Tiles = std::vector<Tile>;

struct HSegment {
	// Horizontal segment, a closed interval
	int left;
	int right;
	int row;

	bool operator<(const HSegment& other) const noexcept
	{
		if (row != other.row)
			return row < other.row;
		return left < other.left;
	}
};

std::ostream& operator<<(std::ostream& os, const HSegment& segment)
{
	os << "(" << segment.left << "-" << segment.right << "," << segment.row << ")";
	return os;
}

using HSegments = std::set<HSegment>;

struct Block {
	int left;
	int right;
	// Remember that Y grows downside
	int bottom;
	int top;
	// The block with its boundaries belongs to the red-green field
	bool is_inside;

	bool operator<(const Block& other) const noexcept
	{
		if (left != other.left)
			return left < other.left;
		return bottom < other.bottom;
	}
	
	std::pair<int, int> get_sections(const HSegment& segment) const noexcept
	{
		// Calculate intersection of a segment and a block.
		// Return {-1,-1} if there is no intersection.
		if (segment.right < left || segment.left > right)
			return {-1, -1};
		if (is_inside && (segment.right == left || segment.left == right))
			return {-1, -1};

		return {
			segment.left > left ? segment.left : left,
			segment.right < right ? segment.right : right
		};
	}
};


std::string to_string_with_inf(int v)
{
	if (v == std::numeric_limits<int>::min())
		return "-Inf";
	else if (v == std::numeric_limits<int>::max())
		return "+Inf";
	else
		return std::to_string(v);
}

std::ostream& operator<<(std::ostream& os, const Block& block)
{
	os << (block.is_inside ? "+" : "-")
		<< "[" << to_string_with_inf(block.left) << "-" << to_string_with_inf(block.right) << ","
		<< to_string_with_inf(block.bottom) << "-" << to_string_with_inf(block.top) << "]";
	return os;
}

using Blocks = std::set<Block>;

bool add_block(Blocks& blocks, const Block& block)
{
	if (block.top < block.bottom)
		return false;
	blocks.emplace(block);
	return true;
}

bool intersects_blocks(const Blocks& blocks, const Block& block)
{
	for (const auto& b : blocks) {
		if (block.left > b.right || block.right < b.left)
			continue;

		if (block.bottom > b.top || block.top < b.bottom)
			continue;

		return true;
	}

	return false;
}

Blocks form_blocks(const HSegments& segs)
{
	Blocks blocks = {
		{
			std::numeric_limits<int>::min(),
			std::numeric_limits<int>::max(),
			std::numeric_limits<int>::min(),
			std::numeric_limits<int>::max(),
			false
		}
	};
	Blocks blocks_fixed;
	for (const auto& s : segs) {
		#ifdef AOC_DEBUG
		std::cout << s << '\n';
		#endif

		Blocks blocks_cont;
		// We'll fix left and right boundaries in the end of the loop below
		Block block_new = {
			std::numeric_limits<int>::max(),
			std::numeric_limits<int>::min(),
			std::numeric_limits<int>::min(),
			std::numeric_limits<int>::max(),
			false
		};

		for (auto b = blocks.begin(); b != blocks.end();) {
			auto sections = b->get_sections(s);

			if (sections.first == -1) {
				++b;
				continue;
			}

			int new_left = sections.first;
			int new_right = sections.second;

			Block block_left, block_right, block_fixed;
			bool added_left = false, added_right = false, added_fixed = false;
			(void) added_fixed;

			if (new_left > b->left) {
				block_left = {
					b->left, new_left - (b->is_inside ? 0 : 1), b->bottom, b->top, b->is_inside
				};
				added_left = add_block(blocks_cont, block_left);
			}

			if (new_right < b->right) {
				block_right = {
					new_right + (b->is_inside ? 0 : 1), b->right, b->bottom, b->top, b->is_inside
				};
				added_right = add_block(blocks_cont, block_right);
			}

			{
				block_fixed = {
					added_left ? block_left.right + 1 : new_left,
					added_right ? block_right.left - 1: new_right,
					b->bottom,
					s.row - (b->is_inside ? 0 : 1),
					b->is_inside
				};
				added_fixed = add_block(blocks_fixed, block_fixed);
			}

			#ifdef AOC_DEBUG
			std::cout << "  " << *b << " -> <";
			if (added_left)
				std::cout << block_left;
			std::cout << ", ";
			if (added_fixed)
				std::cout << block_fixed;
			std::cout << ", ";
			if (added_right)
				std::cout << block_right;
			std::cout << ">" << '\n';
			#endif

			block_new.left = std::min(block_new.left, block_fixed.left);
			block_new.right = std::max(block_new.right, block_fixed.right);
			block_new.bottom = block_fixed.top + 1;
			block_new.is_inside = !b->is_inside;

			b = blocks.erase(b);
		}

		blocks.insert(blocks_cont.begin(), blocks_cont.end());

		bool added_new = add_block(blocks, block_new);
		(void) added_new;

		#ifdef AOC_DEBUG
		if (added_new)
			std::cout << "  <= " << block_new << '\n';
		std::cout << '\n';
		#endif
	}

	// Add few remaining blocks left in "blocks" container
	blocks_fixed.insert(blocks.begin(), blocks.end());

	return blocks_fixed;
}


int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	Tiles tiles;

	while (file >> record) {
		auto [t0, t1] = aoc::parse_pair<int>(record, ",");
		tiles.emplace_back(Tile{t0, t1});
	}

	auto ntiles = tiles.size();

	HSegments segs;
	auto tprev = ntiles - 1;
	for (std::size_t t = 0; t < ntiles; t++) {
		if (tiles[tprev][1] == tiles[t][1]) {
			auto left_right = std::minmax(tiles[tprev][0], tiles[t][0]);
			segs.emplace(HSegment{left_right.first, left_right.second, tiles[t][1]});
		}
		tprev = t;
	}

	Blocks blocks = form_blocks(segs);

	#ifdef AOC_DEBUG
	for (const auto& b : blocks)
		std::cout << b << '\n';
	std::cout << '\n';
	#endif

	Blocks blocks_out;
	std::copy_if(blocks.begin(), blocks.end(),
		std::inserter(blocks_out, blocks_out.end()),
		[](const auto& b) { return !b.is_inside; }
	);

	std::vector<std::pair<Block, i64>> areas;
	areas.reserve(ntiles * (ntiles-1) / 2);
	for (std::size_t i = 0; i < ntiles; i++) {
		for (std::size_t j = i+1; j < ntiles; j++) {
			// We are lazy and just reuse Block struct
			std::pair<int, int> lr = std::minmax(tiles[i][0], tiles[j][0]);
			std::pair<int, int> bt = std::minmax(tiles[i][1], tiles[j][1]);
			Block b = {lr.first, lr.second, bt.first, bt.second, true};

			bool fits = !intersects_blocks(blocks_out, b);

			auto area =
				static_cast<i64>(b.right - b.left + 1) *
				static_cast<i64>(b.top - b.bottom + 1);

			#ifdef AOC_DEBUG
			std::cout << (fits ? "* " : "  ") << b << " " << area << '\n';
			#endif

			if (!fits)
				continue;
			areas.emplace_back(std::pair<Block, i64>{b, area});
		}
	}

	std::nth_element(areas.begin(), areas.begin(), areas.end(),
		[](const auto& a, const auto& b) {return a.second > b.second;}
	);

	#ifdef AOC_DEBUG
	const Block& win = areas[0].first;
	std::cout << "Winner: " << win << '\n';
	// This is suitable format for visualizer
	std::cout << win.left << " " << win.bottom << " " << win.right << " " << win.top << '\n';
	#endif

	auto result = areas[0].second;
	std::cout << "Result: " << result << '\n';

	return 0;
}
