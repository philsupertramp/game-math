#include "forwardSub.h"


std::vector<float> forwardSub(const std::vector<std::vector<float>>& L, const std::vector<float>& b) {
    size_t m = L.size();
    size_t n = L[0].size();
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

    auto c = zerosV(v);

    c[0] = b[0];

    for(size_t j = 1; j < m; j++) {
        float s_k = 0.0f;
        for(size_t k = 0; k < j; k++) { s_k = s_k + (L[j][k] * c[k]); }
        c[j] = b[j] - s_k;
    }

    return c;
}
