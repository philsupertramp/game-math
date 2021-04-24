/**
 * LU decomposition, places L and U in resulting matrix
 *
 * A = L * U
 *
 * L = [
 *        1, 0, 0, ...
 *        l_{2,1}, 1, 0, ...
 *        l_{3,1}, l_{3,2}, 1, ...
 *        ...
 *        l_{m,1}, ...
 *  ]
 *  U = [
 *        0, u_{1,2}, u_{1,3}, ..., u_{1,n}
 *        0, 0, u_{2,3}, ..., u_{2,n}
 *        0, ..., 0, u_{m-1, n}
 *        0, ..., 0
 *  ]
 *  Result: L + U
 *  LU decomposition
 */

#pragma once
#include "../../Matrix.h"
#include <cmath>
#include <vector>


std::pair<Matrix<double>, std::vector<unsigned int>> LU(const Matrix<double>& A) {
    auto m = A.rows();
    auto n = A.columns();
    Matrix<double> B = A;

    if(m != n) {
        // Error
    }

    // init pivot vector
    std::vector<unsigned int> p(m, 0);
    for(size_t i = 0; i < m; i++) { p[i] = i; }

    for(size_t col = 0; col < n - 1; col++) {
        auto maxVal = std::abs(B(col, col));
        auto index  = col;
        for(size_t q = col; q < n; q++) {
            if(std::abs(B(q, col)) > maxVal) {
                maxVal = std::abs(B(q, col));
                index  = q;
            }
        }
        auto safe = p[index];
        p[index]  = p[col];
        p[col]    = safe;

        if(index != col) {
            auto matrixSafe = B(col);

            B.SetRow(col, B(index));
            B.SetRow(index, matrixSafe);
        }

        for(size_t row = col + 1; row < n; row++) { B(row, col) /= B(col, col); }

        for(size_t i = col + 1; i < m; i++) {
            for(size_t j = col + 1; j < n; j++) { B(i, j) -= (B(i, col) * B(col, j)); }
        }
    }

    return { B, p };
}
