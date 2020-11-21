#pragma once

#include "../Test.h"
#include <cassert>
#include <math/ode/ode45.h>

bool TestOde45()
{
    auto ode                     = []([[maybe_unused]] float t, std::vector<float> y) { return y; };
    std::vector<float> tInterval = { 0.0f, 5.0f };
    std::vector<float> y0        = { 5.0f };
    float h                      = 1.0f;

    auto foo = ode45(ode, tInterval, y0, h);

    auto yResult = foo.first;
    auto tResult = foo.second;

    float tExpected[6] = { 0, 1, 2, 3, 4, 5 };
    float yExpected[6] = { 5.0000, 13.5916691, 36.9466896, 100.433411, 273.011505, 742.136353 };
    for(int i = 0; i < 6; i++)
    {
        assert(tExpected[i] == tResult[i]);
        assert(yExpected[i] == yResult[i][0]);
    }
    return true;
}


bool TestOde45RB()
{
    auto ode = []([[maybe_unused]] float t, std::vector<float> y) {
        std::vector<float> result;
        result.reserve(y.size());

        auto alpha = 0.25;
        auto beta  = -0.01;
        auto gamma = -1.0;
        auto delta = 0.01;

        result[0] = alpha * y[0] + beta * y[0] * y[1];
        result[1] = gamma * y[1] + delta * y[0] * y[1];
        return result;
    };
    std::vector<float> tInterval = { 0.0f, 5.0f };
    std::vector<float> y0        = { 80, 30 };
    float h                      = 1.0f;
    auto foo                     = odeEulerExp(ode, tInterval, y0, h);

    auto yResult = foo.first;
    auto tResult = foo.second;

    float yExpected[6][2] = {
        { 80.0000, 30.0000 }, { 76, 24 }, { 76.7600021, 18.2399998 },
        { 81.9489746, 14.0010242 }, { 90.9625244, 11.4736958 }, { 103.266388, 10.4367638 },
    };
    float tExpected[6] = { 0, 1, 2, 3, 4, 5 };
    for(int i = 0; i < 6; i++)
    {
        assert(tExpected[i] == tResult[i]);
        assert(yExpected[i][0] == yResult[i][0]);
        assert(yExpected[i][1] == yResult[i][1]);
    }
    return true;
}
