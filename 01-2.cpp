#include "common.hpp"

int main(int argc, char *argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	auto pos = 50;
	auto result = 0;

	while(file >> record) {
		auto dir = record[0];
		auto amount = std::stoi(record.substr(1));

		if (dir == 'L') {
			amount = -amount;
			// To guarantee that the residue is negative
			pos = (pos - 100) % 100;
		}
		//std::cout << record << "\t" << pos << " " << amount << std::endl;

		pos += amount;
		result += std::abs(pos) / 100;

		// We need to bring pos to a positive value, hence additional +100.
		pos = ((pos % 100) + 100) % 100;

		//std::cout << "\t" << pos << " " << result << std::endl;
	}

	std::cout << "Result: " << result << std::endl;

	return 0;
}
