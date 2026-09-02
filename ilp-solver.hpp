#ifndef ILP_SOLVER_HPP
#define ILP_SOLVER_HPP

#include "10-common.hpp"

using Matrix = std::vector<std::vector<int>>;

std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

std::vector<int> solve(Matrix problem);

#endif //ILP_SOLVER_HPP
