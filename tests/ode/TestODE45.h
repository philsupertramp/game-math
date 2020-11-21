#pragma once

#include "../Test.h"
#include <math/ode/ode45.h>
#include <cassert>

bool TestOde45(){

    auto ode = []([[maybe_unused]] float t, std::vector<float> y){
        std::vector<float> result;
        result.reserve(y.size());
        for(size_t i = 0; i < y.size(); i++){
            result[i] = y[i];
        }
        return result;
    };
    std::vector<float> tInterval = {0.0f, 5.0f};
    std::vector<float> y0 = {5.0f};
    float h = 1.0f;

    auto foo = ode45(ode, tInterval, y0, h);

    auto yResult = foo.first;
    auto tResult = foo.second;

    float tExpected[6] = {0, 1, 2, 3, 4, 5};
    float yExpected[6] = {5.0000, 17.4518, 60.9132, 212.6083, 742.0768 };
    for(int i = 0; i < 5; i++){
        assert(tExpected[i] == tResult[i]);
        assert(yExpected[i] == yResult[i][0]);
    }
    return true;
}


/**

    float yExpected[6][2] = {80.0000   30.0000
   78.3609   24.2687
   80.8476   19.7304
   86.6224   16.7281
   94.9172   15.2263
  104.8112   15.1928
    };
    float tExpected[6] = {0, 1, 2, 3, 4, 5};

 */