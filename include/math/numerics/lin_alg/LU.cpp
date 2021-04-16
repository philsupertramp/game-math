#include "LU.h"

std::pair<Matrix<double>, std::vector<unsigned int>> LU(const Matrix<double>& A) {
    auto m = A.rows();
    auto n = A.columns();

    if(m != n) {
        // Error
    }

    // init pivot vector
    std::vector<unsigned int> p(m, 0);
    for(size_t i = 0; i < m; i++) { p[i] = i; }

    for(size_t col = 0; col < n - 1; col++) {
        auto maxVal = std::abs(A(col, col));
        auto index  = col;
        for(size_t q = col; q < n; q++) {
            if(std::abs(A(q, col)) > maxVal) {
                maxVal = std::abs(A(q, col));
                index  = q;
            }
        }
        auto safe = p[index];
        p[index]  = p[col];
        p[col]    = safe;

        auto matrixSafe = A(col);
        for(size_t i = 0; i < n; ++i) {
            A(col, i)   = A(index, i);
            A(index, i) = matrixSafe(0, i);
        }
        for(size_t row = col + 1; row < n; row++) { A(row, col) = A(row, col) / A(col, col); }

        for(size_t i = col + 1; i < m; i++) {
            for(size_t j = col + 1; j < n; j++) { A(i, j) = A(i, j) - A(i, col) * A(col, j); }
        }
    }

    return { A, p };
}