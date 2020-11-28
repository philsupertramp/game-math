#pragma once
#include <vector>
#include <math/numerics/lin_alg/LU.h>

bool TestLU(){

    std::vector<std::vector<float>> A =  {{ 4, 2, 3 }, { 2, 2, 1 }, { 2, 2, 2 }};

    auto result = LU(A);

    std::vector<unsigned int> p = {0, 1, 2};
    std::vector<std::vector<float>> LR = {{ 4.f, 2.f, 3.f }, { 0.5f, 1.f, -0.5f }, { 0.5f, 1.0f, 1.f  }};

    for(size_t col = 0; col < 3; col++){
        assert(result.second[col] == p[col]);
        for(size_t row = 0; row < 3; row++){
            assert(result.first[col][row] == LR[col][row]);
        }
    }
    return true;
}