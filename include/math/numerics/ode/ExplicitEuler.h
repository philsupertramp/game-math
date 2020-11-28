#pragma once

#include "../utils.h"
#include "ode.h"
#include <functional>
#include <vector>

ODEResult odeEulerExp(const ODE& fun, const std::vector<float>& tInterval, const std::vector<float>& y0, float h = 0);
