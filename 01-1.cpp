#include "common.hpp"

int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	auto pos = 50;
	auto result = i64{0};

	while (file >> record) {
		auto dir = record[0];
		auto amount = std::stoi(record.substr(1));

		if (dir == 'L')
			pos -= amount;
		else
			pos += amount;
		pos = pos % 100;

		if (0 == pos)
			result++;
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
