/**
 * Gaussian elimination to solve systems of linear equations.
 * Uses column major pivot elements to reduce number of operations.
 */

#include "gaussSeidel.h"

Matrix<double> gaussSeidel(const Matrix<double>& A, const Matrix<double>& b) {
    auto LR = LU(A);

    auto bCopy = b;
    for(size_t i = 0; i < b.rows(); i++) { b(i, 0) = bCopy(LR.second[i], 0); }

    auto c = forwardSub(LR.first, b);
    return backwardSub(LR.first, c);
}