#pragma once

#include "../../Matrix.h"
#include "ExplicitEuler.h"
#include "ode.h"
#include "ode45.h"
#include "odeBDF2.h"
#include "odeTrapez.h"

/**
 * Proxy interface class for ODE solvers with static member functions as bridge
 * to solvers.
 */
class ODESolver
{
public:
    /**
     * proxy to ODEExpEuler
     * @param fun
     * @param tInterval
     * @param y0
     * @param option
     * @return
     */
    static ODEResult odeExpEuler(
    const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, const ODEOption& option) {
        return ODEExpEuler(fun, tInterval, y0, option.h);
    }
    /**
     * proxy to ODE45
     * @param fun
     * @param tInterval
     * @param y0
     * @param option
     * @return
     */
    static ODEResult
    ode45(const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, const ODEOption& option) {
        return ODE45(fun, tInterval, y0, option.h);
    }
    /**
     * proxy to ODETrapez
     * @param fun
     * @param tInterval
     * @param y0
     * @param option
     * @return
     */
    static ODEResult
    odeTrapez(const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, const ODEOption& option) {
        return ODETrapez(fun, tInterval, y0, option);
    }
    /**
     * proxy to ODEBDF2
     * @param fun
     * @param tInterval
     * @param y0
     * @param option
     * @return
     */
    static ODEResult
    odeBDF2(const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, const ODEOption& option) {
        return ODEBDF2(fun, tInterval, y0, option);
    }
};