#pragma once
#include <functional>
#include <vector>

using ODE    = std::function<Matrix<double>(double, Matrix<double>)>;
using ODEJac = std::function<Matrix<double>(double, Matrix<double>)>;

struct ODEResult {
    Matrix<double> Y;
    Matrix<double> T;
    Matrix<int> Iterations;

    ODEResult(const Matrix<double>& y, const Matrix<double>& t)
        : Y(y)
        , T(t) { }
    ODEResult(const Matrix<double>& y, const Matrix<double>& t, const Matrix<int>& iter)
        : Y(y)
        , T(t)
        , Iterations(iter) { }
};
struct ODEOption {
    double h    = 0;
    double TOL  = 1e-7;
    int maxIter = 50;
    ODEJac Jac  = nullptr;
};