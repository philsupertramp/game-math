/**
 * @file
 *
 * Gaussian elimination to solve systems of linear equations.
 * Uses column major pivot elements to reduce number of operations.
 *
 * Requires:
 * \code
 * #include <math/numerics/lin_alg/gausseidel.h>
 * \endcode
 */
#pragma once
#include "../../Matrix.h"
#include "LU.h"
#include "backwardSub.h"
#include "forwardSub.h"
#include <vector>

/**
 * Gauss-Jordan algorithm to calculate inverse of given squared matrix.
 *
 *
 * @param A squared Coefficient matrix
 * @return $$A^{-1}$$
 */
Matrix<double> gaussJordan(const Matrix<double>& A) {
    /*
     *
# Applying Guass Jordan Elimination
for i in range(n):
    if a[i][i] == 0.0:
        sys.exit('Divide by zero detected!')

    for j in range(n):
        if i != j:
            ratio = a[j][i]/a[i][i]

            for k in range(2*n):
                a[j][k] = a[j][k] - ratio * a[i][k]

# Row operation to make principal diagonal element to 1
for i in range(n):
    divisor = a[i][i]
    for j in range(2*n):
        a[i][j] = a[i][j]/divisor
     */
    auto B = HorizontalConcat(A, eye(A.rows(), A.columns()));
    for(size_t i = 0; i < A.rows(); ++i){
        if(A(i, i) == 0.0f){
            // error
            std::cerr << "DIVISION BY 0!!! for " << i << "=" << A(i, i) << std::endl;
            std::cerr << A << std::endl;
        }
        for(size_t j = 0; j < A.columns(); ++j){
            if(i != j) {
                auto ratio = A(j, i) / A(i, i);
                for(size_t k = j; k < B.columns(); ++k) { B(j, k) = B(j, k) - ratio * B(i, k); }
            }
        }
    }
    for(size_t i = 0; i < B.rows(); ++i) {
        auto factor = B(i, i);
        for(size_t j = 0; j < B.columns(); ++j) {
            B(i, j) = B(i, j) / factor;
        }
    }
    return B.GetSlice(0, B.rows()-1, A.columns(), B.columns()-1);
}

/**
 * \example numerics/lin_alg/TestGaussSeidel.cpp
 * This is an example on how to use gaussSeidel.
 */