/**
 * @file
 *
 * Implements explicit Euler-Method to solve ordinary differential equations.
 */
#pragma once

#include "../utils.h"
#include "ode.h"
#include <functional>
#include <vector>

/**
 * implementation of explicit euler method
 * @param fun
 * @param tInterval
 * @param y0
 * @param h
 * @return
 */
ODEResult ODEExpEuler(const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, double h = 0.0) {
    size_t dim       = tInterval.size();
    size_t elem_size = y0.columns();
    if(h == 0) { h = (tInterval[dim - 1] - tInterval[0]) / 1000; }

    //    % initialize result vectors
    auto result_dim = ((tInterval[dim - 1] - tInterval[0]) / h) + 1;
    Matrix<double> t(0, result_dim, 1, 1);
    size_t n = result_dim;
    auto y   = zeros(n, elem_size);

    t(0, 0) = tInterval[0];
    for(unsigned long i = 0; i < y0.columns(); ++i) y(0, i) = y0(0, i);

    for(size_t i = 1; i < n; i++) {
        auto cur_t               = t(i - 1, 0);
        Matrix<double> cur_y     = y(i - 1);
        Matrix<double> fun_value = fun(cur_t, cur_y);

        auto yi = cur_y + fun_value * h;
        for(size_t j = 0; j < elem_size; j++) { y(i, j) = yi(0, j); }
        t(i, 0) = cur_t + h;
    }
    return { y, t };
}