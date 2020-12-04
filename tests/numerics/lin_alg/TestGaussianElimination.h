#pragma once
#include <cassert>
#include <math/numerics/lin_alg/gaussianElimination.h>
#include <vector>

bool TestGaussianElimination() {
    std::vector<std::vector<float>> A = { { 4, 2, 3 }, { 2, 2, 1 }, { 2, 2, 2 } };
    std::vector<float> b              = { 2, 1, 2 };

    auto result = gaussianElimination(A, b);

    std::vector<float> expectedX = { -0.5f, 0.5f, 1.0f };
    for(size_t i = 0; i < expectedX.size(); i++) { assert(result[i] == expectedX[i]); }
    return true;
}