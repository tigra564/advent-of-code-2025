#include "10-common.hpp"

std::pair<unsigned int, int> get_num_toggles(const Machine& machine) noexcept
{
	const auto& buttons = machine.buttons;
	std::pair<unsigned int, int> result = {0, buttons.size()+1};

	for (auto v = 0U; v < 1U << buttons.size(); v++) {
		auto inds = machine.indicators;
		auto nbuttons = 0;

		for (std::size_t i = 0; i < buttons.size(); i++) {
			if (v & (1U << i)) {
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


int main(int argc, char* argv[])
{
	std::ifstream file = aoc::open_data(argc, argv);
	std::string record;

	std::vector<Machine> machines;

	while (std::getline(file, record)) {
		machines.emplace_back(Machine(record));
	}

	auto result = i64{0};
	for (const auto& m : machines) {
		auto r = get_num_toggles(m);
		result += r.second;
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
