#include "gaussianElimination.h"

std::vector<float> gaussianElimination(std::vector<std::vector<float>> A, std::vector<float> b)
{
    auto LR = LU(A);

    auto bCopy = b;
    for(size_t i = 0; i < b.size(); i++) { b[i] = bCopy[LR.second[i]]; }

    auto c = forwardSub(LR.first, b);
    return backwardSub(LR.first, c);
}