#include "ode45.h"


ODEResult ode45(const ODE& fun, const std::vector<float>& tInterval, const std::vector<float>& y0, float h) {
    size_t dim       = tInterval.size();
    size_t elem_size = y0.size();
    if(h == 0) { h = (tInterval[dim - 1] - tInterval[0]) / 1000; }

    //    % initialize result vectors
    auto result_dim = ((tInterval[dim - 1] - tInterval[0]) / h) + 1;
    std::vector<float> t;
    t.resize(result_dim);
    size_t n                          = result_dim;
    std::vector<std::vector<float>> y = zeros(n, elem_size);

    // clang-format off
    float a[7][7] = {
    {0, 0, 0, 0, 0, 0, 0},
    {1.f/5.f, 0, 0, 0, 0, 0, 0},
    {3.f/40.f, 9.f/40.f, 0, 0, 0, 0, 0},
    {44.f/45.f, -56.f/15.f,  32.f/9.f, 0, 0, 0, 0},
    {19372.f/6561.f, -25360.f/2187.f,  64448.f/6561.f, -212.f/729.f, 0, 0, 0},
    {9017.f/3168.f, -355.f/33.f, 46732.f/5247.f, 49.f/176.f, -5103.f/18656.f, 0, 0},
    {35.f/384.f, 0, 500.f/1113.f,  125.f/192.f, -2187.f/6784.f, 11.f/84.f, 0}
    };

    float c[7]={0, 1.f/5.f, 3.f/10.f, 4.f/5.f, 8.f/9.f, 1.f, 1.f};

    float b5[6]={35.f/384.f, 0, 500.f/1113.f,  125.f/192.f, -2187.f/6784.f, 11.f/84.f};
    [[maybe_unused]] float b4[7]={5179.f/57600.f, 0, 7571.f/16695.f, 393.f/640.f, -92097.f/339200.f, 187.f/2100.f, 1.f/40.f};

    // clang-format on

    y[0] = y0;
    t[0] = tInterval[0];
    for(size_t l = 0; l < n - 1; l++) {
        t[l + 1] = t[l] + h;
        auto k   = zeros(6, elem_size);
        for(size_t i_k = 0; i_k < 6; i_k++) {
            std::vector<float> y_k = y[l];

            for(size_t j = 0; j < i_k; j++) {
                for(size_t elem = 0; elem < elem_size; elem++) { y_k[elem] += +h * a[i_k][j] * k[j][elem]; }
            }
            k[i_k] = fun(t[l] + c[i_k] * h, y_k);
        }
        for(size_t elem = 0; elem < elem_size; elem++) {
            // clang-format off
            y[l + 1][elem] = (y[l][elem]
                              + h * (b5[0] * k[0][elem] + b5[1] * k[1][elem]
                                     + b5[2] * k[2][elem] + b5[3] * k[3][elem]
                                     + b5[4] * k[4][elem] + b5[5] * k[5][elem])); // clang-format on
        }
    }
    return { y, t };
}