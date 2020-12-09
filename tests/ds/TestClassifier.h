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
    Classifier<1, 1> C;

    C.Train<360, 150, 140>("../../tests/ds/", 1000, 2.0, 0.01, 0.1);
    C.Train<360, 150, 140>("../../tests/ds/", 2000, 1.0, 0.01, 0.1, true);
    C.Train<360, 150, 140>("../../tests/ds/", 10000, 1.0, 0.01, 0.1, true);
//    C.Train<75, 37, 38>("../../resources/iris_data_files/", 1000, 0.5, 0.1, 0.2);
//    Classifier<9, 2> C2;
//    C2.Train<350, 175, 174>("../../resources/cancer/set1/", 10000, 1/sqrt(350), 0.01);
    auto res = C.Evaluate(MatrixDS<1,1,double>(0));
    auto res2 = C.Evaluate(MatrixDS<1,1,double>(1));
    auto res3 = C.Evaluate(MatrixDS<1,1,double>(2));
    auto res4 = C.Evaluate(MatrixDS<1,1,double>(6));
    auto res5 = C.Evaluate(MatrixDS<1,1,double>(5.5));
    return true;
}

bool TestSigmoid() {
    assert(sigmoid(1.0) == 0.88079707797788243);
    assert(sigmoid(2.0) == 0.98201379003790845);
    assert(sigmoid(3.0) == 0.99752737684336523);
    assert(sigmoid(4.0) == 0.99966464986953352);
    assert(sigmoid(-1.0) == 0.11920292202211757);
    return true;
}
bool TestSigmoidDx() {
    assert(sigmoidDx(1.0) == 0.20998717080701307);
    assert(sigmoidDx(2.0) == 0.035325412426582214);
    assert(sigmoidDx(3.0) == 0.0049330185827201056);
    assert(sigmoidDx(4.0) == 0.00067047534151293275);
    assert(sigmoidDx(-1.0) == 0.20998717080701307);
    return true;
}
bool TestActivate() {
    MatrixDS<2, 2> A({ { 1, 2 }, { 3, 4 } });
    MatrixDS<2, 2> C({ { 0.88079707797788243, 0.98201379003790845 }, { 0.99752737684336523, 0.99966464986953352 } });
    auto B = Activate(A);

    assert(C == B);

    MatrixDS<2, 2> D(1);
    MatrixDS<2, 2> E(0.88079707797788243);

    auto F = Activate(D);
    assert(F == E);
    return true;
}
bool TestActivateDerivative() {
    MatrixDS<2, 2> A({ { 1, 2 }, { 3, 4 } });
    MatrixDS<2, 2> C(
    { { 0.20998717080701307, 0.035325412426582214 }, { 0.0049330185827201056, 0.00067047534151293275 } });
    auto B = ActivateDerivative(A);

    assert(B == C);

    MatrixDS<2, 2> D(1);
    MatrixDS<2, 2> D2(-1);
    MatrixDS<2, 2> E(0.20998717080701307);

    auto F  = ActivateDerivative(D);
    auto F2 = ActivateDerivative(D2);
    assert(F == E);
    assert(F2 == E);
    return true;
}

bool TestFeedForward() { return true; }

bool TestEvaluationStatistics() {
    EvaluationStatistics stats;

    return true;
}
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
    TestSigmoid();
    TestActivate();
    TestSigmoidDx();
    TestActivateDerivative();
    TestClassifierConstruction();
    TestInitializeWeights();
    return true;
}