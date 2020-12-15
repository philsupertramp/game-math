#pragma once

#include "../Test.h"
#include <cassert>
#include <math/ds/MatrixDS.h>
#include <math/ds/NN.h>


bool TestClassifier() {
    auto nn = NN<2,2>();
    nn.AddLayer<4,2>();
    auto res = nn.FeedForward();
    return true;
}