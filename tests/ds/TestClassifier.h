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

    C.Train(750, 1.0/2.0, 0.1);
    assert(C.finalErrors.Training.Regression == 0.21519289280901185);
    assert(C.finalErrors.Training.Classification == 0.41333333333333333);
    assert(C.finalErrors.Validation.Regression == 0.24089840689879136);
    assert(C.finalErrors.Validation.Classification == 0.21621621621621623);
    assert(C.finalErrors.Test.Regression == 0.22624711842133263);
    assert(C.finalErrors.Test.Classification == 0.23684210526315788);
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
    MatrixDS<2, 2> C({ { 0.20998717080701307, 0.035325412426582214 }, { 0.0049330185827201056, 0.00067047534151293275 } });
    auto B = ActivateDerivative(A);

    assert(B == C);

    MatrixDS<2, 2> D(1);
    MatrixDS<2, 2> D2(-1);
    MatrixDS<2, 2> E(0.20998717080701307);

    auto F = ActivateDerivative(D);
    auto F2 = ActivateDerivative(D2);
    assert(F == E);
    assert(F2 == E);
    return true;
}

bool TestFeedForward() {

    return true;
}

bool TestEvaluationStatistics(){
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