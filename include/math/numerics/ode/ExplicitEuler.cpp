#include "ExplicitEuler.h"


ODEResult odeEulerExp(const ODE& fun, const std::vector<float>& tInterval, const std::vector<float>& y0, float h) {
    size_t dim       = tInterval.size();
    size_t elem_size = y0.size();
    if(h == 0) { h = (tInterval[dim - 1] - tInterval[0]) / 1000; }

    //    % initialize result vectors
    auto result_dim = ((tInterval[dim - 1] - tInterval[0]) / h) + 1;
    std::vector<float> t;
    t.resize(result_dim);
    size_t n                          = result_dim;
    std::vector<std::vector<float>> y = zeros(n, elem_size);

    t[0] = tInterval[0];
    y[0] = y0;

    for(size_t i = 1; i < n; i++) {
        auto cur_t                   = t[i - 1];
        std::vector<float> cur_y     = y[i - 1];
        std::vector<float> fun_value = fun(cur_t, cur_y);

        y[i].resize(elem_size);
        for(size_t j = 0; j < elem_size; j++) { y[i][j] = cur_y[j] + fun_value[j] * h; }
        t[i] = cur_t + h;
    }
    return { y, t };
}