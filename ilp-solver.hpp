#ifndef AOC_ILP_SOLVER_HPP
#define AOC_ILP_SOLVER_HPP

#include "10-common.hpp"

using Matrix = std::vector<std::vector<int>>;

std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

std::vector<int> solve(Matrix problem);

#endif //AOC_ILP_SOLVER_HPP
