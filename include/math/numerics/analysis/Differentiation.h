/**
 * @file Differentiation.h
 *
 * Numerical differentiation methods to approximate the first order derivative using support values.
 *
 * $$f'(x) = ?$$
 *
 * Implemented quotients:
 * - Forward
 * $$
 *  \frac{f(x+h)-f(x)}{h}
 * $$
 * - Backward
 * $$
 *  \frac{f(x)-f(x-h)}{h}
 * $$
 * - Center
 * $$
 *  \frac{f(x+h)-f(x-h)}{2h}
 * $$
 *
 * Usage:
 * \code
 * // create a function
 * auto fun = [](const Matrix<double>& input){ return input.Apply([](const double& val){ return val*val; });};
 *
 * // create support values
 * auto xk = linspace(-5, 5, 20).Transpose();
 *
 * // calculate 1st derivative
 * centralDiff(xk, fun(xk));
 * \endcode
 *
 * \image html difference_quotients.svg width=50%
 */
#pragma once

/**
 * Computes forward difference quotient
 * $$
 *  \frac{f(x+h)-f(x)}{h}
 * $$
 * **Error** O(dx)
 * @param x x-values
 * @param y y-values
 * @returns approximated differential evaluated on x-values
 */
Matrix<double> forwardDiff(const Matrix<double>& x, const Matrix<double>& y) {
    auto df  = zeros(x.rows(), x.columns());
    auto dy  = y.GetSlice(1, y.rows() - 1, 0, y.columns() - 1) - y.GetSlice(0, y.rows() - 2, 0, y.columns() - 1);
    auto dx  = x.GetSlice(1, x.rows() - 1, 0, x.columns() - 1) - x.GetSlice(0, x.rows() - 2, 0, x.columns() - 1);
    auto res = HadamardDiv(dy, dx);
    for(size_t i = 0; i < df.rows() - 1; ++i) { df.SetRow(i, res.GetSlice(i, i, 0, res.columns() - 1)); }
    return df;
}

/**
 * Computes backward difference quotient
 * $$
 *  f'(x) = \frac{f(x)-f(x-h)}{h}
 * $$
 * **Error** O(dx)
 * @param x x-values
 * @param y y-values
 * @returns approximated first differential evaluated on x-values
 */
Matrix<double> backwardDiff(const Matrix<double>& x, const Matrix<double>& y) {
    auto df  = zeros(x.rows(), x.columns());
    auto dy  = y.GetSlice(1, y.rows() - 1, 0, y.columns() - 1) - y.GetSlice(0, y.rows() - 2, 0, y.columns() - 1);
    auto dx  = x.GetSlice(1, x.rows() - 1, 0, x.columns() - 1) - x.GetSlice(0, x.rows() - 2, 0, x.columns() - 1);
    auto res = HadamardDiv(dy, dx);
    for(size_t i = 1; i < df.rows(); ++i) { df.SetRow(i, res.GetSlice(i, i, 0, res.columns() - 1)); }
    return df;
}

/**
 * Computes central difference quotient
 * $$
 *  f'(x) = \frac{f(x+h)-f(x-h)}{2h}
 * $$
 * **Error** O(dx^2)
 * @param x x-values
 * @param y y-values
 * @returns approximated first differential evaluated on x-values
 */
Matrix<double> centralDiff(const Matrix<double>& x, const Matrix<double>& y) {
    auto df  = zeros(x.rows(), x.columns());
    auto dy  = y.GetSlice(2, y.rows() - 1, 0, y.columns() - 1) - y.GetSlice(0, y.rows() - 3, 0, y.columns() - 1);
    auto dx  = x.GetSlice(2, x.rows() - 1, 0, x.columns() - 1) - x.GetSlice(0, x.rows() - 3, 0, x.columns() - 1);
    auto res = HadamardDiv(dy, dx);
    for(size_t i = 1; i < df.rows() - 1; ++i) { df.SetRow(i, res.GetSlice(i, i, 0, res.columns() - 1)); }
    return df;
}

/**
 * Computes backward difference quotient for 1st differential
 * uses 2nd order polynomial for approximation
 *
 * **Error** O(dx)
 * @param x x-values
 * @param y y-values
 * @returns approximated second differential evaluated on x-values
 */
Matrix<double> backwardDiff2(const Matrix<double>& x, const Matrix<double>& y) {
    auto df = zeros(x.rows(), x.columns());
    auto dy = (3 * y.GetSlice(2, y.rows() - 1, 0, y.columns() - 1))
              - (4 * y.GetSlice(1, y.rows() - 2, 0, y.columns() - 1)) + y.GetSlice(0, y.rows() - 3, 0, y.columns() - 1);
    auto dx  = x.GetSlice(2, x.rows() - 1, 0, x.columns() - 1) - x.GetSlice(0, x.rows() - 3, 0, x.columns() - 1);
    auto res = HadamardDiv(dy, dx);
    for(size_t i = 2; i < df.rows(); ++i) { df.SetRow(i, res.GetSlice(i, i, 0, res.columns() - 1)); }
    return df;
}

/**
 * Computes central difference quotient for 1st differential
 * uses 2nd order polynomial for approximation
 *
 * **Error** O(dx^2)
 * @param x x-values
 * @param y y-values
 * @returns approximated second differential evaluated on x-values
 */
Matrix<double> centralDiff4(const Matrix<double>& x, const Matrix<double>& y) {
    auto df = zeros(x.rows(), x.columns());
    auto dy = (-1 * y.GetSlice(4, y.rows() - 1, 0, y.columns() - 1))
              + (8 * y.GetSlice(3, y.rows() - 2, 0, y.columns() - 1))
              - (8 * y.GetSlice(1, y.rows() - 4, 0, y.columns() - 1)) - y.GetSlice(0, y.rows() - 5, 0, y.columns() - 1);
    auto dx  = 3 * (x.GetSlice(4, x.rows() - 1, 0, x.columns() - 1) - x.GetSlice(0, x.rows() - 5, 0, x.columns() - 1));
    auto res = HadamardDiv(dy, dx);
    for(size_t i = 2; i < df.rows() - 2; ++i) { df.SetRow(i, res.GetSlice(i, i, 0, res.columns() - 1)); }
    return df;
}

/**
 * \example numerics/analysis/TestDifferentiation.cpp
 * This is an example on how to use the Differentiation file.
 */