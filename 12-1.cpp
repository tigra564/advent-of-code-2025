#include <sstream>

#include "common.hpp"

int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

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

	std::cout << "Result: " << result << '\n';

	return 0;
}
