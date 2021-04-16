#include "odeBDF2.h"
#include "odeTrapez.h"
#include <iostream>

ODEResult
odeBDF2(const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, const ODEOption& options) {
    size_t tDim      = tInterval.size();
    size_t elem_size = y0.columns();
    auto h           = options.h;
    auto TOL         = options.TOL;
    auto maxIter     = options.maxIter;
    auto Jac         = options.Jac;
    if(h == 0) { h = (tInterval[tDim - 1] - tInterval[0]) / 1000.0; }

    //    % initialize result vectors
    size_t result_dim = ((tInterval[tDim - 1] - tInterval[0]) / h) + 1;
    std::vector<double> t(result_dim, 0);
    size_t n            = result_dim;
    auto y              = zeros(n, elem_size);
    Matrix<double> iter = zeros(n, 1);

    t[0] = tInterval[0];
    t[1] = t[0] + h;

    auto y1 = odeTrapez(fun, { t[0], t[1] }, y0, options).first;
    y.SetRow(0, y1(0));
    y.SetRow(1, y1(1));
    auto E = eye(elem_size);

    for(size_t l = 1; l < n - 1; l++) {
        auto y_act = y(l - 1).Transpose();
        auto k     = 0;
        auto delta = ones(elem_size);

        while(norm(delta) > TOL && k < maxIter) {
            auto current_fun = fun(t[l] + h, y_act);
            auto F =
            -1.0 * ((-1.0 / 2.0) * y(l - 1).Transpose() + h * current_fun + 2.0 * y(l).Transpose() - 3.0 / 2.0 * y_act);
            auto J = h * Jac(t[l], y_act) - (3.0 / 2.0) * E;
            delta  = gaussSeidel(J, F);
            y_act += delta;
            k += 1;
        }
        if(k >= maxIter) { std::cout << "Warning: Max number iterations reached." << std::endl; }
        iter(l, 0) = k;
        t[l + 1]   = t[l] + h;
        y.SetRow(l + 1, y_act);
    }
    return { y, t };
}