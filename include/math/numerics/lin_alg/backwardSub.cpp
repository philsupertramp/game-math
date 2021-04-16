#include "backwardSub.h"


Matrix<double> backwardSub(const Matrix<double>& R, const Matrix<double>& b) {
    size_t m = R.rows();
    size_t n = R.columns();
    size_t v = b.rows();
    if(v != m) {
        // Error, cannot compute
        std::cout << "Matrix vector dimension miss match, error!\n";
        return Matrix<double>();
    }
    if(m != n) {
        // Error, cannot compute
        std::cout << "Matrix not squared. Dimension miss match, error!\n";
        return Matrix<double>();
    }

    auto x = zerosV(v);

    if(R(m - 1, n - 1) != 0) { x(v - 1, 0) = b(v - 1, 0) / R(m - 1, n - 1); }

    for(int j = (int)m - 2; j >= 0; j--) {
        double s_k = 0.0f;
        for(size_t k = j + 1; k < m; k++) { s_k = s_k + (R(j, k) * x(k, 0)); }
        x(j, 0) = (b(j, 0) - s_k) / R(j, j);
    }

    return x;
}
