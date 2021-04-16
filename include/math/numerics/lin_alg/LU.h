/**
 * In place LU decomposition, places L and U in resulting matrix
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
 *  in-place LU decomposition
 */

#pragma once
#include "../../Matrix.h"
#include <cmath>
#include <vector>

std::pair<Matrix<double>, std::vector<unsigned int>> LU(const Matrix<double>& A);
