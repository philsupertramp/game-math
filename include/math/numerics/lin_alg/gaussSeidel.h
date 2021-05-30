/**
 * @file
 *
 * Gaussian elimination to solve systems of linear equations.
 * Uses column major pivot elements to reduce number of operations.
 */
#pragma once
#include "../../Matrix.h"
#include "LU.h"
#include "backwardSub.h"
#include "forwardSub.h"
#include <vector>

/**
 * Gauss-Seidel algorithm to evaluate system of linear equations.
 *
 * $$Ax = b$$
 *
 * @param A Coefficient matrix
 * @param b resulting vector
 * @return $$x$$
 */
Matrix<double> gaussSeidel(const Matrix<double>& A, const Matrix<double>& b) {
    auto LR = LU(A);

    auto bCopy = b;
    for(size_t i = 0; i < b.rows(); i++) { b(i, 0) = bCopy(LR.second[i], 0); }

    auto c = forwardSub(LR.first, b);
    return backwardSub(LR.first, c);
}

/**
 * \example numerics/lin_alg/TestGaussSeidel.cpp
 * This is an example on how to use gaussSeidel.
 */