#pragma once

#include "../Test.h"
#include <math/ds/Classifier.h>

bool TestInToOutConversion(){
    const MatrixDS<5, 3> A({{1,0,0},{0,1,0},{0,0,1},{1,0,0},{0,0,1}});
    const MatrixDS<5, 1> B({{0}, {1}, {2}, {0}, {2}});
    auto resultIn = OutputToClass(A);
    assert(resultIn == B);
    auto resultOut = ClassToOutput<3>(B);
    assert(resultOut == A);
    return true;
}

bool TestClassifierConstruction(){
    Classifier<4, 3> C;

    C.Train();
    return true;
}

bool TestSigmoid(){

    return true;
}
bool TestSigmoidDx(){
    return true;
}
bool TestActivate(){
    MatrixDS<2,2> A({{1,2},{3,4}});
    MatrixDS<2,2> C({{0.88079707797788243, 0.98201379003790845},{0.99752737684336523, 0.99966464986953352}});
    auto B = Activate(A);

    assert(C == B);
    return true;
}
bool TestActivateDerivative(){

    return true;
}

bool TestFeedForward(){

    return true;
}
bool TestInitializeWeights(){

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
bool TestClassifier(){
    TestInToOutConversion();
    TestActivate();
    TestClassifierConstruction();
    TestInitializeWeights();
    return true;
}