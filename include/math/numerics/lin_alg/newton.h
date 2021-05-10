/**
 * @file
 *
 * Implements newton-method to find roots
 */

#pragma once
#include "../utils.h"
#include "gaussSeidel.h"
#include <functional>

//! representation of jacobian
using Jacobian = std::function<Matrix<double>(const Matrix<double>&)>;
//! representation of linear equation
using LinearEquation = std::function<Matrix<double>(const Matrix<double>&)>;

/**
 * newton method to find roots of given function f
 * @param f
 * @param Df
 * @param x0
 * @param TOL
 * @param maxIter
 * @return
 */
Matrix<double> newton(const LinearEquation& f, const Jacobian& Df, const Matrix<double>& x0, double TOL, int maxIter) {
    auto m = x0.rows();
    if(m < 1) {
        // error
    }
    auto x = x0;

    int iter = 0;
    double r = TOL + 1;

    while(r > TOL && iter < maxIter) {
        auto D_F = Df(x);
        auto F   = f(x) * -1.0;

        auto delta = gaussSeidel(D_F, F);

        x += delta;

        r = norm(delta);
        iter += 1;
    }
    if(iter == maxIter) {
        // error max Iter reached, no convergence
    }
    return x;
}
