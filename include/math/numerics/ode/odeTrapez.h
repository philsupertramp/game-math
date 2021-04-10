#pragma once

#include "ode.h"
#include "../utils.h"
#include "../lin_alg/newton.h"

ODEResult odeTrapez(const ODE& fun, const std::vector<float>& tInterval, const std::vector<float>& y0, const ODEOption& option);
