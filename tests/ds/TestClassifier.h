#pragma once

#include "../Test.h"
#include <cassert>
#include <math/ds/Classifier.h>


bool TestInToOutConversion() {
    const MatrixDS<5, 3> A({ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 0, 1 } });
    const MatrixDS<5, 1> B({ { 0 }, { 1 }, { 2 }, { 0 }, { 2 } });
    auto resultIn = OutputToClass(A);
    assert(resultIn == B);
    auto resultOut = ClassToOutput<3>(B);
    assert(resultOut == A);
    return true;
}

bool TestClassifierConstruction() {
    Classifier<4, 3, 75, 37, 38> C;

    C.Train(500, 0.5, 0.1);
    return true;
}

bool TestSigmoid() { return true; }
bool TestSigmoidDx() { return true; }
bool TestActivate() {
    MatrixDS<2, 2> A({ { 1, 2 }, { 3, 4 } });
    MatrixDS<2, 2> C({ { 0.88079707797788243, 0.98201379003790845 }, { 0.99752737684336523, 0.99966464986953352 } });
    auto B = Activate(A);

    assert(C == B);

    MatrixDS<2,2> D(1);
    MatrixDS<2,2> E(0.88079707797788243);

    auto F = Activate(D);
    assert(F == E);
    return true;
}
bool TestActivateDerivative() { return true; }

bool TestFeedForward() { return true; }
bool TestInitializeWeights() {
    MatrixDS<2, 2, double> A;
    InitializeWeights(A, 0.0, 1.0);
    std::cout << A;
    InitializeWeights(A, 0.0, 1.0);
    std::cout << A;
    InitializeWeights(A, 0.0, 1.0);
    std::cout << A;
    InitializeWeights(A, 0.0, 1.0);
    std::cout << A;
    return true;
}
bool TestClassifier() {
    TestInToOutConversion();
    TestActivate();
    TestClassifierConstruction();
    TestInitializeWeights();
    return true;
}