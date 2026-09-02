#include <iomanip>

#include "ilp-solver.hpp"


std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
{
	int width = 2;
	for (const auto& row : matrix) {
		for (const auto& cell : row) {
			width = std::max(width, static_cast<int>(std::to_string(cell).size()));
		}
	}

	for (const auto& row : matrix) {
		for (const auto& cell : row) {
			std::cout << std::setw(width + 1) << cell; 
		}
		std::cout << std::endl;
	}

	return os;
}


static void scale_down(std::vector<int>& line)
{
	auto ncols = line.size();

	// Scale row so that basis variable coefficient is 1.  If possible.
	// In any case its sign must be positive.
	size_t col = 0;
	for (; col < ncols; col++) {
		if (line[col])
			break;
	}

	int coeff = line[col];
	if (1 == coeff)
		return;
	int sign = coeff > 0 ? 1 : -1;
	int mingcd = std::numeric_limits<int>::max();
	if (-1 == coeff)
		mingcd = 1;
	else
		for (size_t c = col + 1; c < ncols; c++)
			mingcd = std::min(mingcd, std::gcd(coeff, line[c]));
	for (size_t c = col; c < ncols; c++)
		line[c] /= sign * mingcd;
}


static int gauss_forward_step(Matrix& matrix, int row, int col)
{
	int ncols = static_cast<int>(matrix[0].size());

	// We start from <row,col> and search downwards for the minimum
	// non-zero cell (equal to +-1 ideally).  If there is no such a cell
	// then the variable with index equal to row is not a basis one.  Thus
	// we must continue with the next column starting again with the row
	// supplied by the caller. Sooner or later we'll find something since
	// our firther steps guarantee that all rows are not empty.
	while (col < ncols - 1) {
		int min_abs_coeff = std::abs(matrix[row][col]);
		if (1 == min_abs_coeff)
			break;
		else if (0 == min_abs_coeff)
			min_abs_coeff = std::numeric_limits<int>::max();

		for (size_t swap_row = row + 1; swap_row < matrix.size(); swap_row++) {
			if (min_abs_coeff == 1)
				break;

			int curr_abs_coeff = std::abs(matrix[swap_row][col]);
			if (0 != curr_abs_coeff && curr_abs_coeff < min_abs_coeff) {
				std::swap(matrix[swap_row], matrix[row]);
				min_abs_coeff = curr_abs_coeff;
			}
		}
		if (min_abs_coeff < std::numeric_limits<int>::max())
			break;
		col++;
	}

	scale_down(matrix[row]);

	// Go through current column and set all coefficients to 0 (excluding
	// current row).
	// If some row goes all zeroes we delete it. Rows above the row we are
	// now in cannot zero out.
	const auto main_line_it = matrix.begin() + row;
	const auto& main_line = *main_line_it;
	auto main_coeff = main_line[col]; // It's more than 0, but may be more than 1
	for (auto it = matrix.begin(); it != matrix.end();) {
		if (it == main_line_it) {
			it++;
			continue;
		}

		auto& cur_line = *it;

		auto curr_coeff = cur_line[col];
		auto gcd = std::gcd(main_coeff, curr_coeff);
		auto main_factor = curr_coeff / gcd;
		auto curr_factor = main_coeff / gcd;

		auto all_zeroes = true;

		for (int c = 0; c < ncols; c++) {
			cur_line[c] = cur_line[c] * curr_factor - main_line[c] * main_factor;
			if (cur_line[c])
				all_zeroes = false;
		}
		if (all_zeroes) {
			it = matrix.erase(it);
		} else {
			scale_down(cur_line);
			it++;
		}
	}

	return col;
}


static int equalize_basis(Matrix& problem)
{
	std::set<int> factors;
	std::vector<int> factors_initial;

	for (const auto& line : problem) {
		const auto it = std::find_if(line.begin(), line.end(),
			[](auto x) { return x != 0; }
		);
		factors.emplace(*it);
		factors_initial.emplace_back(*it);
	}

	auto all = std::accumulate(factors.begin(), factors.end(),
			1, std::multiplies<int>());

	for (size_t row = 0; row < problem.size(); row++) {
		auto f = factors_initial[row];
		for (auto& cell : problem[row])
			cell *= all / f;
	}

	return all;
}


// We use only upper bounds, lower bounds are always 0.
static std::vector<int> find_bounds(const Matrix& free_vars)
{
	auto num_vars = free_vars[0].size() - 1;
	const auto MAXINT = std::numeric_limits<int>::max();
	std::vector<int> bounds(num_vars, MAXINT);

	for (const auto& line : free_vars) {
		int rhs = line.back();
		for (size_t v = 0; v < num_vars; v++) {
			auto coeff = line[v];
			// We process only vars with strictly positive coefficients
			if (coeff <= 0)
				continue;

			// coeff * var + rest <= rhs
			// var <= (rhs - rest) / coeff
			const auto it = std::find_if(line.rbegin() + 1, line.rend(),
				[](auto x) { return x < 0; }
			);
			// If some else variable is present with a negative
			// sign then we can't fix upper limit on the current
			// variable.
			if (it == line.rend())
				bounds[v] = std::min(bounds[v], rhs / coeff);
		}
	}

	std::cout << "Bounds: " << bounds << std::endl;

	size_t unbounded;
	for (unbounded = 0; unbounded < num_vars; unbounded++) {
		if (MAXINT == bounds[unbounded])
			break;
	}

	if (unbounded == num_vars)
		return bounds;

	// Some bounds stay unlimited, so, substitute bounded free variables with their upper bounds and re-run.
	// Thus we take out one unbounded free variables at a pass.
	Matrix new_free_vars = free_vars;

	for (auto& line : new_free_vars) {
		// No unbounded var in this inequality
		if (0 == line[unbounded])
			continue;

		for (size_t b = 0; b < num_vars; b++) {
			// Skip unbounded variables
			if (bounds[b] == MAXINT)
				continue;

			// We take a variable into account only if it comes
			// with a negative factor. Variables with positive
			// factors can always be set to 0 thus rendering no
			// effect on bounds.
			// In any case the factor for the variable is zeroed.

			if (line[b] < 0)
				line.back() -= bounds[b] * line[b];
			line[b] = 0;
		}
	}
	std::cout << "New free variables\n" << new_free_vars << std::endl;

	return find_bounds(new_free_vars);
}


static std::vector<int> fix_free_vars(Matrix& free_vars, int basis_factor)
{
	auto num_vars = free_vars[0].size() - 1;
	// In the specific case of our riddle the objective function is
	// x_1 + x_2 + ... + x_n, i.e. every variable is counted once. Hence
	// initialization with 1's.
	// But this is true if basis variables equal 1. Otherwise we have to
	// account for this initializing all objective coefficients with
	// basis_factor.
	std::vector<int> objective_coeffs(num_vars, basis_factor);

	for (const auto& line : free_vars) {
		for (size_t c = 0; c < num_vars; c++) {
			objective_coeffs[c] -= line[c];
		}
	}
	std::cout << "Objective coeffs: " << objective_coeffs << std::endl;

	auto bounds = find_bounds(free_vars);

	std::vector<int> best_solution;
	int best_objective = std::numeric_limits<int>::max();

	std::function<void(const std::vector<int>&)> try_free_vars_combo;
	try_free_vars_combo = [
		num_vars, basis_factor,
		&free_vars, &objective_coeffs, &bounds, &best_solution, &best_objective,
		&try_free_vars_combo
	](const std::vector<int>& values)
	{
		//std::cout << "Values: " << values << std::endl;
		
		if (values.size() < num_vars) {
			std::vector<int> new_values = values;
			new_values.emplace_back(0);
			auto var_pos = values.size();
			//std::cout << "New values: " << new_values << std::endl;
			for (auto val = 0; val <= bounds[var_pos]; val++) {
				new_values.back() = val;
				try_free_vars_combo(new_values);
			}

			return;
		}

		for (const auto& line : free_vars) {
			int lhs = 0;
			for (size_t v = 0; v < num_vars; v++) {
				lhs += line[v] * values[v];
			}
			auto diff = line.back() - lhs;
			if (0 > diff)
				return;
			if (diff % basis_factor)
				return;
		}

		int objective = 0;
		for (size_t v = 0; v < num_vars; v++) {
			objective += objective_coeffs[v] * values[v];
		}

		//std::cout << "F = " << objective << std::endl;
		if (objective < best_objective) {
			best_objective = objective;
			best_solution = values;
		}
	};

	try_free_vars_combo({});
	std::cout << best_objective << ": " << best_solution << std::endl;

	return best_solution;
}


std::vector<int> solve(Matrix problem)
{
	std::set<int> basis_var_poses, free_var_poses;

	std::cout << problem << std::endl;

	int col = 0;
	for (size_t row = 0; row < problem.size(); row++) {
		col = gauss_forward_step(problem, row, col);
		basis_var_poses.emplace(col);
		col++;
		std::cout << problem << std::endl;
	}

	auto bfactor = equalize_basis(problem);
	std::cout << "Equalized:\n" << problem << std::endl;

	for (size_t c = 0; c < problem[0].size() - 1; c++)
		if (basis_var_poses.find(c) == basis_var_poses.end())
			free_var_poses.emplace(c);

	std::cout << "Basis variable positions: " << basis_var_poses << std::endl;
	std::cout << "Free  variable positions: " << free_var_poses << std::endl;

	Matrix free_vars;
	for (const auto& line : problem) {
		std::vector<int> free_line;
		free_line.reserve(free_var_poses.size() + 1);

		for (const auto& pos : free_var_poses) {
			free_line.emplace_back(line[pos]);
		}
		free_line.emplace_back(line.back());
		free_vars.emplace_back(free_line);
	}
	std::cout << free_vars << std::endl;

	std::vector<int> fv_solution = fix_free_vars(free_vars, bfactor);
	std::vector<int> solution(problem[0].size() - 1, 0);
	int fv_pos = 0;
	for (const auto& p : free_var_poses) {
		solution[p] = fv_solution[fv_pos];
		fv_pos++;
	}
	int row = 0;
	for (const auto& bvp: basis_var_poses) {
		int rhs = problem[row].back();
		fv_pos = 0;
		for (const auto& fvp : free_var_poses) {
			rhs -= fv_solution[fv_pos] * problem[row][fvp];
			fv_pos++;
		}
		solution[bvp] = rhs / bfactor;
		row++;
	}

	return solution;
}
