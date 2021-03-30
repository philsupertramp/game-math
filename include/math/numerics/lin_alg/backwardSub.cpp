#include "backwardSub.h"


std::vector<float> backwardSub(const std::vector<std::vector<float>>& R, const std::vector<float>& b) {
    size_t m = R.size();
    size_t n = R[0].size();
    size_t v = b.size();
    if(v != m) {
        // Error, cannot compute
        std::cout << "Matrix vector dimension miss match, error!\n";
        return {};
    }
    if(m != n) {
        // Error, cannot compute
        std::cout << "Matrix not squared. Dimension miss match, error!\n";
        return {};
    }

    auto x = zerosV(v);

    if(R[m - 1][n - 1] != 0) { x[v - 1] = b[v - 1] / R[m - 1][n - 1]; }

    for(int j = m - 2; j >= 0; j--) {
        float s_k = 0.0f;
        for(size_t k = j + 1; k < m; k++) { s_k = s_k + (R[j][k] * x[k]); }
        x[j] = (b[j] - s_k) / R[j][j];
    }

    return x;
}
