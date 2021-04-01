#include "odeTrapez.h"
#include <iostream>

ODEResult odeTrapez(const ODE& fun, const std::vector<float>& tInterval, const std::vector<float>& y0, const ODEOption& options){
    size_t dim       = tInterval.size();
    size_t elem_size = y0.size();
    auto h = options.h;
    auto TOL = options.TOL;
    auto maxIter = options.maxIter;
    auto Jac = options.Jac;
    if(h == 0) { h = (tInterval[dim - 1] - tInterval[0]) / 1000; }

    //    % initialize result vectors
    auto result_dim = ((tInterval[dim - 1] - tInterval[0]) / h) + 1;
    std::vector<float> t;
    t.resize(result_dim);
    size_t n                          = result_dim;
    std::vector<std::vector<float>> y = zeros(n, elem_size);
    std::vector<float> iter = zeros( 1, n)[0];
    auto E = eye(n);

    // clang-format on

    y[0] = y0;
    t[0] = tInterval[0];
    for(size_t l = 1; l < n - 1; l++) {
        auto y_act = y[l-1];
        t[l] = t[l-1] + h;
        auto k   = 0;
        auto delta = ones(elem_size);

        auto prev_fun = fun(t[l-1], y_act);

        while(norm(delta) > TOL && k < maxIter){
            auto current_fun = fun(t[l], y_act);
            auto F = zeros(1, y_act.size())[0];
            for(int i = 0; i < y_act.size(); ++i) {
                F[i] = -(y[l-1][i] + h/2 * (prev_fun[i] + current_fun[i]) - y_act[i]);
            }
            auto J = Jac(t[l], y_act);
            for(int i = 0; i < J.size(); ++i) {
                for(int j = 0; j < J[i].size(); ++j) {
                    J[i][j] = J[i][j] * h/2 - E[i][j];
                }
            }
            delta[0] = newton(J, F);
            for(int i = 0; i < delta[0].size(); ++i) {
                y_act[i] += delta[0][i];
            }
            k += 1;
            // TODO: might be irrelevant
            prev_fun = current_fun;

        }
        if(k >= maxIter){
            std::cout << "Warning: Max number iterations reached." << std::endl;
        }
        iter[l] = k;
        for(size_t elem = 0; elem < elem_size; elem++) { y[l][elem] = y_act[elem]; }

    }
    return { y, t };
}