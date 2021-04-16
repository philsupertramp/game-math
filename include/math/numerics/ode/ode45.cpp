#include "ode45.h"


ODEResult ode45(const ODE& fun, const std::vector<double>& tInterval, const Matrix<double>& y0, double h) {
    size_t dim       = tInterval.size();
    size_t elem_size = y0.columns();
    if(h == 0) { h = (tInterval[dim - 1] - tInterval[0]) / 1000; }
    auto result_dim = int((tInterval[dim - 1] - tInterval[0]) / h) + 1;

    std::vector<double> t(result_dim, 0);
    size_t n         = result_dim;
    Matrix<double> y = zeros(n, elem_size);

    Matrix<double> a({ { 0, 0, 0, 0, 0, 0, 0 },
                       { 1.0 / 5.0, 0, 0, 0, 0, 0, 0 },
                       { 3.0 / 40.0, 9.0 / 40.0, 0, 0, 0, 0, 0 },
                       { 44.0 / 45.0, -56.0 / 15.0, 32.0 / 9.0, 0, 0, 0, 0 },
                       { 19372.0 / 6561.0, -25360.0 / 2187.0, 64448.0 / 6561.0, -212.0 / 729.0, 0, 0, 0 },
                       { 9017.0 / 3168.0, -355.0 / 33.0, 46732.0 / 5247.0, 49.0 / 176.0, -5103.0 / 18656.0, 0, 0 },
                       { 35.0 / 384.0, 0, 500.0 / 1113.0, 125.0 / 192.0, -2187.0 / 6784.0, 11.0 / 84.0, 0 } });

    Matrix<double> c({ { 0, 1.0 / 5.0, 3.0 / 10.0, 4.0 / 5.0, 8.0 / 9.0, 1.0, 1.0 } });

    Matrix<double> b5({ { 35.0 / 384.0, 0, 500.0 / 1113.0, 125.0 / 192.0, -2187.0 / 6784.0, 11.0 / 84.0 } });
    [[maybe_unused]] Matrix<double> b4(
    { { 5179.0 / 57600.0, 0, 7571.0 / 16695.0, 393.0 / 640.0, -92097.0 / 339200.0, 187.0 / 2100.0, 1.0 / 40.0 } });


    y.SetRow(0, y0);
    t[0] = tInterval[0];
    for(size_t l = 0; l < n - 1; l++) {
        t[l + 1] = t[l] + h;
        auto k   = zeros(6, elem_size);
        for(size_t i_k = 0; i_k < 6; i_k++) {
            Matrix<double> y_k = y(l);

            for(size_t j = 0; j < i_k; j++) {
                for(size_t elem = 0; elem < elem_size; elem++) { y_k(0, elem) += h * a(i_k, j) * k(j, elem); }
            }
            k.SetRow(i_k, fun(t[l] + c(0, i_k) * h, y_k));
        }
        auto y_l = y(l) + (b5 * k) * h;
        y.SetRow(l + 1, y_l);
    }
    return { y, t };
}