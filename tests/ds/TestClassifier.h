#pragma once

#include "../Test.h"
#include <cassert>
#include <math/ds/MatrixDS.h>
#include <math/ds/NN.h>


bool TestClassifier() {
    NN<3,1> nn(5);
    nn.SetLayers(5);
    nn.Train();
    return true;
}