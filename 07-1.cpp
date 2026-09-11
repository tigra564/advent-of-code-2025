#include "common.hpp"

int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	file >> record;
	std::string beams(record.size(), ' ');
	auto spos = record.find('S');
	beams[spos] = '|';

	auto result = i64{0};
	while (file >> record) {
		for (std::size_t i = 0; i < record.size(); i++) {
			if ('^' == record[i] && '|' == beams[i]) {
				result++;
				beams.replace(i-1, 3, "| |");
			}
		}
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
