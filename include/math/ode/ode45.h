#pragma once

#include <vector>
#include <functional>
#include "../ode.h"

ODEResult ode45(const ODE& fun, const std::vector<float>& tInterval, const std::vector<float>& y0, float h = 0) {

    size_t dim = tInterval.size();
    size_t elem_size = y0.size();
    if(h == 0){
        h = (tInterval[dim-1] - tInterval[0])/1000;
    }

//    % initialize result vectors
    auto result_dim = ((tInterval[dim-1] - tInterval[0])/h)+1;
    auto t = linspace(tInterval[0], tInterval[dim-1], result_dim);
    size_t n = result_dim;
    std::vector<std::vector<float>> y = zeros(n, elem_size);

    float a[8][7] = {
            {0.f,            0.f,             0.f,            0.f,          0.f,             0.f,       0.f},
            {0.f,            0.f,             0.f,            0.f,          0.f,             0.f,       0.f},
            {1.f / 5.f,        0.f,             0.f,            0.f,          0.f,             0.f,       0.f},
            {3.f / 40.f,       9.f / 40.f,        0.f,            0.f,          0.f,             0.f,       0.f},
            {44.f / 45.f,      -56.f / 15.f,      32.f / 9.f,       0.f,          0.f,             0.f,       0.f},
            {19372.f / 6561.f, -25360.f / 2187.f, 64448.f / 6561, -212.f / 729.f, 0.f,             0.f,       0.f},
            {9017.f / 3168.f,  -355.f / 33.f,     46732.f / 5247, 49.f / 176.f,   -5103.f / 18656.f, 0.f,       0.f},
            {35.f / 384.f,     0.f,             500.f / 1113.f,   125.f / 192.f,  -2187.f / 6784.f,  11.f / 84.f, 0.f}
    };
    float c[7]={0.f, 1.f/5.f, 3.f/10.f, 4.f/5.f, 8.f/9.f, 1.f, 1.f};

    [[maybe_unused]] float b4[6]= {35.f/384.f, 0.f, 500.f/1113.f,  125.f/192.f, -2187.f/6784.f, 11.f/84.f};
    float b5[7]={5179.f/57600.f, 0.f, 7571.f/16695.f, 393.f/640.f, -92097.f/339200.f, 187.f/2100.f, 1.f/40.f};

    y[0] = y0;

    for(size_t i = 0; i < n-1; i++){
        std::vector<std::vector<float>> k;
        k.resize(6);
        for(size_t j = 0; j<6;j++){
            auto y_k = y[i];
            for(size_t q = 0; q<j; q++){
                for(size_t v = 0; v<elem_size; v++) {
                    y_k[v] += a[j][q] * k[q][v];
                }
            }
            k[j] = fun(t[i] + c[j] * h, y_k);
        }

        y[i+1] = y[i];
        for(size_t j = 0; j<6; j++){
            for(size_t v = 0; v<elem_size; v++) {
                y[i + 1][v] += h * b5[0] * k[j][v];
            }
        }

//        y_exakt=y(l,:) + h* (b4(1)*k1+b4(2)*k2+b4(3)*k3+b4(4)*k4+b4(5)*k5+b4(6)*k6+b4(7)*k7);

    }
    return {y,t};
}