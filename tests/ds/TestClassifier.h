#pragma once

#include "../Test.h"
#include <cassert>
#include <math/ds/NNModel.h>


bool TestInToOutConversion() {
    const MatrixDS<5, 3> A({ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 0, 1 } });
    const MatrixDS<5, 1> B({ { 1 }, { 2 }, { 3 }, { 1 }, { 3 } });
    auto resultIn = OutputToClass(A);
    assert(resultIn == B);
    auto resultOut = ClassToOutput<3>(B);
    assert(resultOut == A);
    return true;
}

bool TestClassifierConstruction() {
    NNModel<4, 3> C;

//    C.Train<360, 150, 140>("../../tests/ds/", 1000, 0.5, 0.1, 0.1, false, true);
    C.Train<75, 37, 38>("../../resources/iris_data_files/", 1000, 0.5, 1.0, 0.1, false, true);
//    Classifier<9, 2> C2;
//    C2.Train<350, 175, 174>("../../resources/cancer/set1/", 10000, 1/sqrt(350), 0.01);
    auto res = C.Predict(MatrixDS<1,4,double>(0)); // 0
    auto res2 = C.Predict(MatrixDS<1,4,double>({{0.790235, 0.480944,	0.82379,	0.828996}})); // 3
    auto res3 = C.Predict(MatrixDS<1,4,double>({{0.928809,	0.339785,	0.975,	0.901998}})); // 3
    auto res4 = C.Predict(MatrixDS<1,4,double>({{0.328323,	0.304495,	0.385282,	0.463986}})); // 2
    auto res5 = C.Predict(MatrixDS<1,4,double>({{0.305227,	0.586813,	0.143347,	0.135476}})); // 1
//    auto res2 = C.Predict(MatrixDS<1,1,double>(1));
//    auto res3 = C.Predict(MatrixDS<1,1,double>(2));
//    auto res4 = C.Predict(MatrixDS<1,1,double>(6));
//    auto res5 = C.Predict(MatrixDS<1,1,double>(5.5));
    return true;
}
bool TestSigmoid(){
    assert(sigmoid(1.0) == 0.7310585786300049);
    assert(sigmoid(2.0) == 0.8807970779778823);
    assert(sigmoid(3.0) == 0.9525741268224334);
    assert(sigmoid(4.0) == 0.9820137900379085);
    assert(sigmoid(-1.0) == 0.2689414213699951);
    return true;
}

bool TestSigmoidDx(){
    assert(sigmoidDx(1.0) == 0.0);
    assert(sigmoidDx(2.0) == -2.0);
    assert(sigmoidDx(3.0) == -6.0);
    assert(sigmoidDx(4.0) == -12.0);
    assert(sigmoidDx(5.0) == -20.0);
    assert(sigmoidDx(-1.0) == -2.0);
    assert(sigmoidDx(-2.0) == -6.0);
    assert(sigmoidDx(-3.0) == -12.0);
    assert(sigmoidDx(-4.0) == -20.0);
    assert(sigmoidDx(-5.0) == -30.0);
    return true;
}
bool TestTanh() {
    assert(Tanh(1.0) == 0.88079707797788243);
    assert(Tanh(2.0) == 0.98201379003790845);
    assert(Tanh(3.0) == 0.99752737684336523);
    assert(Tanh(4.0) == 0.99966464986953352);
    assert(Tanh(-1.0) == 0.11920292202211757);
    return true;
}
bool TestTanhDx() {
    assert(TanhDx(1.0) == 0.20998717080701307);
    assert(TanhDx(2.0) == 0.035325412426582214);
    assert(TanhDx(3.0) == 0.0049330185827201056);
    assert(TanhDx(4.0) == 0.00067047534151293275);
    assert(TanhDx(-1.0) == 0.20998717080701307);
    return true;
}
bool TestActivate() {
    MatrixDS<2, 2> A({ { 1, 2 }, { 3, 4 } });
    MatrixDS<2, 2> C({ { 0.7310585786300049, 0.8807970779778823 }, { 0.9525741268224334, 0.9820137900379085 } });
    auto B = Activate(A);

    assert(C == B);

    MatrixDS<2, 2> D(-1);
    MatrixDS<2, 2> E(0.2689414213699951);

    auto F = Activate(D);
    assert(F == E);
    return true;
}
bool TestActivateDerivative() {
    MatrixDS<2, 2> A({ { 1, 2 }, { 3, 4 } });
    MatrixDS<2, 2> C(
    { { 0.0, -2.0 }, { -6.0, -12.0 } });
    auto B = ActivateDerivative(A);

    assert(B == C);

    MatrixDS<2, 2> D(2);
    MatrixDS<2, 2> D2(-1);
    MatrixDS<2, 2> E(-2.0);

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
//    TestActivate();
    TestSigmoidDx();
//    TestActivateDerivative();
    TestTanh();
    TestTanhDx();
    TestClassifierConstruction();
    TestInitializeWeights();
    return true;
}