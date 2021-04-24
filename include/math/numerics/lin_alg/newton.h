#pragma once
#include "../utils.h"
#include "gaussSeidel.h"
#include <functional>


using Jacobian       = std::function<Matrix<double>(double, const Matrix<double>&)>;
using LinearEquation = std::function<Matrix<double>(double, const Matrix<double>&)>;

#include <functional>

Matrix<double> newton(
const std::function<Matrix<double>(const Matrix<double>&)>& f,
const std::function<Matrix<double>(const Matrix<double>&)>& Df,
const Matrix<double>& x0,
double TOL,
int maxIter) {
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
        // error max Iter reached
    }
    return x;
}
