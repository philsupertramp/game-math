#pragma once

#include <functional>
#include <vector>

using ODE       = std::function<std::vector<float>(float, std::vector<float>)>;
using ODEJac    = std::function<std::vector<std::vector<float>>(float, std::vector<float>)>;
using ODEResult = std::pair<std::vector<std::vector<float>>, std::vector<float>>;

struct ODEOption {
    float h = 0;
    double TOL = 1e-7;
    int maxIter = 50;
    ODEJac Jac;
};
