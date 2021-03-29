#pragma once

#include <cassert>

#include <math/numerics/utils.h>

bool TestLinspace(){

    auto vec = linspace(0, 1, 3);
    assert(vec[0] == 0);
    assert(vec[1] == 0.5);
    assert(vec[2] == 1.0);
    return true;

}
bool TestNumericsUtils(){
    TestLinspace();
    return true;
}
