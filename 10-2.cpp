#include "ilp-solver.hpp"

unsigned int get_num_presses(const Machine& machine)
{
	Matrix problem(machine.num_inds, std::vector<int>{});
	const auto& buttons = machine.buttons;

	for (std::size_t row = 0; row < problem.size(); row++) {
		auto& line = problem[row];
		line.resize(buttons.size() + 1, 0);
		for (std::size_t b = 0; b < buttons.size(); b++) {
			if (buttons[b] & (1 << row))
				line[b] = 1;
		}
		line.back() = machine.joltages[row];
	}

	auto solution = solve(problem); 
	std::cout << "Solution: " << solution << '\n'; 

	int result = 0;
	for (const auto& v : solution)
		result += v;

	std::cout << "Number of presses: " << result << '\n';

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

	auto result = 0ULL;
	for (const auto& m : machines) {
		std::cout << m << '\n';
		auto r = get_num_presses(m);
		std::cout << '\n';

		result += r;
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
