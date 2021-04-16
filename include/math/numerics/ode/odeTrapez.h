#pragma once

#include "../lin_alg/newton.h"
#include "../utils.h"
#include "ode.h"

ODEResult
odeTrapez(const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, const ODEOption& option);
