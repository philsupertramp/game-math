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
    // This is rubbish
    // We need row major pivot in here
    /**
     * Idea is simple,
     * - keep A as is, create copy
     * - generate identity matrix of dimensions of A
     * - potentially check out https://github.com/hbtalha/Matrix-Library, https://www.cplusplus.com/forum/beginner/267832/
     */
//    auto res = LU(A);
    auto B = A;
    for(size_t i = 0; i < B.rows(); ++i){
        if(B(i, i) == 0.0f){
            // error
            std::cerr << "DIVISION BY 0!!! for " << i << "=" << B(i, i) << std::endl;
        }
        for(size_t j = 0; j < B.columns(); ++j){
            if(i != j) {
                auto ratio = B(j, i) / B(i, i);
                for(size_t k = j; k < B.columns(); ++k) { B(j, k) = B(j, k) - ratio * B(i, k); }
            }
        }
    }

    for(size_t i = 0; i < B.rows(); ++i) {
        auto factor = B(i, i);
        for(size_t j = 0; j < B.columns(); ++j) { B(i, j) = B(i, j) / factor; }
    }
    return B;
}

/**
 * \example numerics/lin_alg/TestGaussSeidel.cpp
 * This is an example on how to use gaussSeidel.
 */