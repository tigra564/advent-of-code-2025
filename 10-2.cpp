#include "ilp-solver.hpp"

//#define AOC_DEBUG

unsigned int get_num_presses(const Machine& machine)
{
	Matrix problem(machine.num_inds, std::vector<int>{});
	const auto& buttons = machine.buttons;

	for (std::size_t row = 0; row < problem.size(); row++) {
		auto& line = problem[row];
		line.resize(buttons.size() + 1, 0);
		for (std::size_t b = 0; b < buttons.size(); b++) {
			if (buttons[b] & (1U << row))
				line[b] = 1;
		}
		line.back() = machine.joltages[row];
	}

	auto solution = solve(problem); 
	#ifdef AOC_DEBUG
	std::cout << "Solution: " << solution << '\n'; 
	#endif

	int result = 0;
	for (const auto& v : solution)
		result += v;

	#ifdef AOC_DEBUG
	std::cout << "Number of presses: " << result << '\n';
	#endif

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
		#ifdef AOC_DEBUG
		std::cout << m << '\n';
		#endif

		auto r = get_num_presses(m);

		#ifdef AOC_DEBUG
		std::cout << '\n';
		#endif

		result += r;
	}

	std::cout << "Result: " << result << '\n';

	return 0;
}
