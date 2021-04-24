#pragma once

#include "../lin_alg/newton.h"
#include "../utils.h"
#include "ode.h"
#include <iostream>

ODEResult
ODETrapez(const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, const ODEOption& option) {
    size_t dim       = tInterval.size();
    size_t elem_size = y0.columns();
    auto h           = option.h;
    auto TOL         = option.TOL;
    auto maxIter     = option.maxIter;
    auto Jac         = option.Jac;
    if(h == 0) { h = (tInterval[dim - 1] - tInterval[0]) / 1000.0; }
    int n = int((tInterval[dim - 1] - tInterval[0]) / h) + 1;

    auto t    = Matrix<double>(0, n, 1, 1);
    auto iter = Matrix<int>(0, n, 1);
    auto y    = zeros(n, elem_size);

    t(0, 0) = tInterval[0];
    y.SetRow(0, y0);
    auto E = eye(elem_size);

    for(int i = 1; i < n; ++i) {
        auto y_act    = y(i - 1).Transpose();
        t(i, 0)       = t(i - 1, 0) + h;
        int k         = 0;
        auto delta    = ones(elem_size);
        auto prev_fun = fun(t(i - 1, 0), y_act);

        while(norm(delta) > TOL && k < maxIter) {
            auto current_fun = fun(t(i, 0), y_act);
            auto F           = -1.0 * (y(i - 1).Transpose() + h / 2 * (prev_fun + current_fun) - y_act);
            auto J           = h / 2 * Jac(t(i, 0), y_act) - E;

            delta = gaussSeidel(J, F);
            y_act += delta;
            k++;
        }
        if(k == maxIter) {
            // err max iteration reached, did not convert.
        }
        iter(i, 0) = k;
        y.SetRow(i, y_act);
    }
    return { y, t, iter };
}
