#include "odeBDF2.h"
#include "odeTrapez.h"
#include <iostream>

ODEResult odeBDF2(const ODE& fun, const std::vector<float>& tInterval, const std::vector<float>& y0, const ODEOption& options){
    size_t tDim      = tInterval.size();
    size_t elem_size = y0.size();
    auto h = options.h;
    auto TOL = options.TOL;
    auto maxIter = options.maxIter;
    auto Jac = options.Jac;
    if(h == 0) { h = (tInterval[tDim - 1] - tInterval[0]) / 1000; }

    //    % initialize result vectors
    auto result_dim = ((tInterval[tDim - 1] - tInterval[0]) / h) + 1;
    std::vector<float> t;
    t.resize(result_dim);
    size_t n                          = result_dim;
    std::vector<std::vector<float>> y = zeros(n, elem_size);
    std::vector<float> iter = zeros( 1, n)[0];

    y[0] = y0;
    t[0] = tInterval[0];
    t[1] = t[0] + h;

    y[1] = odeTrapez(fun, {t[0], t[1]}, y0, options).first[0];
    auto E = eye(n);

    for(size_t l = 1; l < n; l++) {
        auto y_act = y[l-1];
        auto k   = 0;
        auto delta = ones(elem_size);

        while(norm(delta) > TOL && k < maxIter){
            auto y2 = y[l];
            for(float & i : y2) {
                i *= 2.0;
            }
            auto current_fun = fun(t[l] + h, y_act);
            auto F = zeros(1, y_act.size())[0];
            for(int i = 0; i < y_act.size(); ++i) {
                F[i] = -(-0.5 * y[l-1][i] + h * current_fun[i] + y2[i] - 1.5 * y_act[i]);
            }
            auto J = Jac(t[l], y_act);
            for(int i = 0; i < J.size(); ++i) {
                for(int j = 0; j < J[i].size(); ++j) {
                    J[i][j] = h * J[i][j] * 1.5 - E[i][j];
                }
            }
            delta[0] = gaussSeidel(J, F);
            for(int i = 0; i < delta[0].size(); ++i) {
                y_act[i] += delta[0][i];
            }
            k += 1;
        }
        if(k >= maxIter){
            std::cout << "Warning: Max number iterations reached." << std::endl;
        }
        iter[l] = k;
        t[l+1] = t[l] + h;
        for(size_t elem = 0; elem < elem_size; elem++) { y[l+1][elem] = y_act[elem]; }

    }
    return { y, t };
}