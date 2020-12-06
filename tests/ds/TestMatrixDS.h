#pragma once

#include "../Test.h"
#include <cassert>
#include <math/ds/MatrixDS.h>


bool TestMatrixDSMultiplication() {
    MatrixDS<3, 2, double> A(1.0);
    MatrixDS<2, 3, double> B(2.0);
    MatrixDS<2, 2, double> C(2.0);
    MatrixDS<2, 2, double> C2(1.0);
    double v = 2.0;
    auto D   = A * v;
    for(size_t i = 0; i < D.columns(); ++i) {
        for(size_t j = 0; j < D.rows(); ++j) { assert(D[i][j] == 2.0); }
    }
    //    B * C; <- not possible
    C* C2;

    MatrixDS<2, 2, double> C3;
    C3 = C * C2;

    for(size_t i = 0; i < C3.columns(); ++i) {
        for(size_t j = 0; j < C3.rows(); ++j) { assert(C3[i][j] == 4.0); }
    }
    auto E = A * B;
    std::cout << E;
    for(size_t i = 0; i < E.columns(); ++i) {
        for(size_t j = 0; j < E.rows(); ++j) { assert(E[i][j] == 4); }
    }

    MatrixDS<10, 1> vec(1.0);
    MatrixDS<1, 10> vec2(2.0);

    auto resVec = vec * vec2;
    assert(resVec.rows() == 10);
    assert(resVec.columns() == 10);
    auto resVec2 = vec2 * vec;
    assert(resVec2.rows() == 1);
    assert(resVec2.columns() == 1);

    MatrixDS<2> vec2A(2.0);
    MatrixDS<2, 2> mat2(1.0);
    MatrixDS<2> vec2b(4.0);
    MatrixDS<2> resA = mat2 * vec2A;

    assert(resA == vec2b);
    return true;
}

bool TestMatrixAddition() {
    MatrixDS<2, 2, double> A(2.0);
    MatrixDS<2, 2, double> B(2.0);
    MatrixDS<2, 2, double> C;
    auto D = A + B;

    MatrixDS<2, 2, double> E(4.0);
    assert(D == E);
    C += A;
    assert(C == B);

    MatrixDS<2> vec2(1.0);
    MatrixDS<2> vec2b(1.0);
    MatrixDS<2> vec2c(2.0);

    assert((vec2 + vec2b) == vec2c);

    // Impossible
    //    vec2 + A;
    return true;
}

bool TestHadamardMultiplication() {
    MatrixDS<2, 2, double> A(1.0);
    MatrixDS<2, 2, double> B(2.015);

    auto C = A.HadamardMulti(B);

    assert(C == B);

    MatrixDS<2, 2, double> D({ { 0, 1 }, { 0, 0 } });
    MatrixDS<2, 2, double> D2({ { 0, 1 }, { 0, 0 } });
    MatrixDS<2, 2, double> E(5.0);
    MatrixDS<2, 2, double> resultA({ { 0, 5.0 }, { 0, 0 } });
    MatrixDS<2, 2, double> resultB({ { 0, 25.0 }, { 0, 0 } });

    assert(D.HadamardMulti(E) == resultA);
    D.HadamardMulti(E);
    assert(D == resultB);

    auto resD2E = HadamardMulti(D2, E);
    assert(resD2E == resultA);

    MatrixDS<2, 5, double> Q1(10.0);
    MatrixDS<3, 5, double> Q2(10.0);
    MatrixDS<5, 2, double> Q3(10.0);

    // Impossible
    //    Q1.HadamardMulti(Q2);
    //    Q1.HadamardMulti(Q3);
    //    HadamardMulti(Q1, Q2);
    return true;
}

bool TestKroneckerMultiplication() {
    MatrixDS<1, 2> A(1.0);
    MatrixDS<2, 1> B(2.0);
    MatrixDS<2, 2> C(2.0);

    auto resA = A.KroneckerMulti(B);

    assert(resA == C);

    MatrixDS<2, 2> A2({ { 1, 2 }, { 3, 4 } });
    MatrixDS<2, 2> B2({ { 0, 5 }, { 6, 7 } });
    MatrixDS<4, 4> C2({ { 0, 5, 0, 10 }, { 6, 7, 12, 14 }, { 0, 15, 0, 20 }, { 18, 21, 24, 28 } });

    MatrixDS<4, 4> resB = A2.KroneckerMulti(B2);

    assert(resB == C2);

    MatrixDS<3, 2> A3({ { 1, 2 }, { 3, 4 }, { 5, 6 } });
    MatrixDS<3, 2> A32({ { 1, 2 }, { 3, 4 }, { 5, 6 } });
    MatrixDS<2, 2> B3({ { 7, 8 }, { 9, 0 } });
    MatrixDS<6, 4> C3(
    { { 7, 8, 14, 16 }, { 9, 0, 18, 0 }, { 21, 24, 28, 32 }, { 27, 0, 36, 0 }, { 35, 40, 42, 48 }, { 45, 0, 54, 0 } });

    auto resC = A3.KroneckerMulti(B3);
    assert(resC == C3);

    auto resC2 = KroneckerMulti(A32, B3);
    assert(resC2 == resC && resC2 == C3);

    return true;
}

bool TestHorizontalConcat() {
    MatrixDS<2, 2> A(1.0);
    MatrixDS<2, 2> A2(1.0);
    MatrixDS<2> B(2.0);
    MatrixDS<2, 3> C({ { 1.0, 1.0, 2.0 }, { 1.0, 1.0, 2.0 } });

    auto res1 = A.HorizontalConcat(B);

    assert(res1 == C);

    auto res2 = HorizontalConcat(A2, B);

    assert(res2 == C);

    return true;
}

bool TestMatrixDSCompare() {
    MatrixDS<2, 2, double> A(2.0);
    MatrixDS<2, 2, double> B(2.0);
    MatrixDS<2, 2, double> C;
    MatrixDS<2, 3, double> D(2.0);
    MatrixDS<3, 2, double> E(2.0);
    MatrixDS<3, 3, double> F({ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } });
    MatrixDS<3, 3, double> G({ { 1, 0, 0 }, { 0, 1, 1 }, { 0, 0, 1 } });

    assert(A == B);
    assert(A != C);
    // cannot be compared by implementation
    //    assert(A != D);
    //    assert(A != E);
    //    assert(D == E);
    assert(F != G);
    assert(F.Determinant() == G.Determinant());

    return true;
}

bool TestMatrixDSDeterminant() {
    MatrixDS<3, 3, double> A({ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } });
    MatrixDS<3, 3, double> B({ { -1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } });
    MatrixDS<3, 3, double> C({ { 3, 5, 2 }, { 8, 4, 8 }, { 2, 4, 7 } });
    MatrixDS<4, 4, double> D({ { 9, 5, 2, 5 }, { 9, 5, 3, 7 }, { 6, 5, 4, 8 }, { 1, 5, 3, 7 } });
    MatrixDS<2, 2, double> E({ { 1, 2 }, { 3, 4 } });
    assert(A.Determinant() == 1.0);
    assert(B.Determinant() == -1.0);
    assert(C.Determinant() == -164);
    assert(D.Determinant() == -40);
    assert(E.Determinant() == -2);
    return true;
}

bool TestMatrixDSTranspose() {
    // Vector
    MatrixDS<1, 3, double> vec({ { 1, 2, 3 } });
    MatrixDS<3, 1, double> vec2 = vec.Transpose();
    assert(vec2.columns() == vec.rows());
    assert(vec2.rows() == vec.columns());
    for(size_t i = 0; i < 3; ++i) { assert(vec[0][i] == vec2[i][0]); }
    // Matrix
    MatrixDS<2, 2, double> A(1);
    assert(A == A.Transpose());
    MatrixDS<2, 2, double> B({ { 1, 1 }, { 2, 2 } });
    MatrixDS<2, 2, double> C({ { 1, 2 }, { 1, 2 } });

    assert(B.Transpose() == C);

    MatrixDS<3, 2, double> D({ { 1, 1 }, { 2, 2 }, { 3, 3 } });
    MatrixDS<2, 3, double> E({ { 1, 2, 3 }, { 1, 2, 3 } });
    assert(D.Transpose() == E);
    return true;
}

bool TestMatrixDSInit() {
    MatrixDS<2, 2, float> matrix;
    assert(matrix.rows() == 2);
    assert(matrix.columns() == 2);

    MatrixDS<10, 100> m2;
    assert(m2.rows() == 10);
    assert(m2.columns() == 100);

    MatrixDS<100, 10> m3;
    assert(m3.rows() == 100);
    assert(m3.columns() == 10);

    MatrixDS<10> m4;
    assert(m4.rows() == 10);
    assert(m4.columns() == 1);

    MatrixDS<2, 2> m5({ { 1.0, 1.0 }, { 1.0, 1.0 } });
    for(size_t i = 0; i < 2; i++) {
        for(size_t j = 0; j < 2; j++) { assert(m5[i][j] == 1.0); }
    }
    MatrixDS<2, 2> m6(1.0);
    for(size_t i = 0; i < 2; i++) {
        for(size_t j = 0; j < 2; j++) { assert(m6[i][j] == 1.0); }
    }
    return true;
}
bool TestMatrixDS() {
    TestMatrixDSInit();
    TestMatrixDSMultiplication();
    TestMatrixAddition();
    TestMatrixDSCompare();
    TestMatrixDSDeterminant();
    TestMatrixDSTranspose();
    TestHadamardMultiplication();
    TestKroneckerMultiplication();
    TestHorizontalConcat();
    return true;
}
