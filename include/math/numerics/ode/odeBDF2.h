#pragma once

#include "../lin_alg/newton.h"
#include "../utils.h"
#include "ode.h"

ODEResult
odeBDF2(const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, const ODEOption& option);
