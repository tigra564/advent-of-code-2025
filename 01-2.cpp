#include "common.hpp"
#include <cmath> // for std::abs

int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	auto pos = 50;
	auto result = i64{0};

	while (file >> record) {
		auto dir = record[0];
		auto amount = std::stoi(record.substr(1));

		if (dir == 'L') {
			// When turning left we apply a negative amount
			amount = -amount;
			// To guarantee that the residue is negative
			pos = (pos - 100) % 100;
		}

		pos += amount;
		result += std::abs(pos) / 100;

		// We need to bring pos to a positive value, hence additional +100.
		pos = ((pos % 100) + 100) % 100;
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
