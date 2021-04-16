#pragma once

#include "../utils.h"
#include "ode.h"
#include <functional>
#include <vector>

ODEResult odeEulerExp(const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, double h = 0);
