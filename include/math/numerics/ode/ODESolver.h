#pragma once

#include "../../Matrix.h"
#include "ExplicitEuler.h"
#include "ode.h"
#include "ode45.h"
#include "odeBDF2.h"
#include "odeTrapez.h"

class ODESolver
{
public:
    static ODEResult odeExplicitEuler(
    const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, const ODEOption& option) {
        return ODEExpEuler(fun, tInterval, y0, option.h);
    }
    static ODEResult
    ode45(const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, const ODEOption& option) {
        return ODE45(fun, tInterval, y0, option.h);
    }
    static ODEResult
    odeTrapez(const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, const ODEOption& option) {
        return ODETrapez(fun, tInterval, y0, option);
    }
    static ODEResult
    odeBDF2(const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, const ODEOption& option) {
        return ODEBDF2(fun, tInterval, y0, option);
    }
};