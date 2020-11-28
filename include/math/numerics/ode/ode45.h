/**
 * ODE Solver
 * implementation based on Dormand-Prince Method to solve ordinary differential equations
 * https://en.wikipedia.org/wiki/Dormand%E2%80%93Prince_method
 *
 * Current state:
 * - implemented 7 function evaluations
 * - using 6/7 evaluations for 5th order RK
 *
 * TODO: use calculated error of 5th order - 4th order to alter step width `h` dynamically to fully implement dp-method.
 */
#pragma once

#include "../utils.h"
#include "ode.h"
#include <functional>
#include <vector>

ODEResult ode45(const ODE& fun, const std::vector<float>& tInterval, const std::vector<float>& y0, float h = 0);
