/**
 * Gaussian elimination to solve systems of linear equations.
 * Uses column major pivot elements to reduce number of operations.
 */

#include "gaussSeidel.h"

std::vector<float> gaussSeidel(std::vector<std::vector<float>> A, std::vector<float> b) {
    auto LR = LU(A);

    auto bCopy = b;
    for(size_t i = 0; i < b.size(); i++) { b[i] = bCopy[LR.second[i]]; }

    auto c = forwardSub(LR.first, b);
    return backwardSub(LR.first, c);
}