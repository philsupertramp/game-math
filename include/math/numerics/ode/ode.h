#pragma once

#include "../../Matrix.h"
#include <functional>
#include <vector>

using ODE       = std::function<Matrix<double>(double, Matrix<double>)>;
using ODEJac    = std::function<Matrix<double>(double, Matrix<double>)>;
using ODEResult = std::pair<Matrix<double>, std::vector<double>>;

struct ODEOption {
    double h    = 0;
    double TOL  = 1e-7;
    int maxIter = 50;
    ODEJac Jac;
};
