#include "10-common.hpp"

std::pair<unsigned int, int> get_num_toggles(const Machine& machine)
{
	const auto& buttons = machine.buttons;
	std::pair<unsigned int, int> result = {0, buttons.size()+1};

	for (auto v = 0; v < 1 << buttons.size(); v++) {
		auto inds = machine.indicators;
		auto nbuttons = 0;

		for (size_t i = 0; i < buttons.size(); i++) {
			if (v & (1 << i)) {
				inds ^= buttons[i];
				nbuttons++;
			}
		}
		if (0 == inds && nbuttons < result.second) {
			result = {v, nbuttons};
		}
	}

	return result;
}


int main(int argc, char *argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	std::vector<Machine> machines;

	while (std::getline(file, record)) {
		machines.emplace_back(Machine(record));
	}

	//for (const auto& m : machines)
	//	std::cout << m << std::endl;

	auto result = 0ULL;
	for (const auto& m : machines) {
		auto r = get_num_toggles(m);
		//std::cout << r.first << " " << r.second << std::endl;
		result += r.second;
	}

	std::cout << "Result: " << result << std::endl;

	return 0;
}
