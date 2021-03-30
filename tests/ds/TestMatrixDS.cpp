#include "../Test.h"
#include <cassert>
#include <math/ds/MatrixDS.h>


class MatrixDSTestCase : public Test
{
    bool TestMatrixDSMultiplication() {
        MatrixDS<double> A(1.0, 3, 2);
        MatrixDS<double> B(2.0, 2, 3);
        MatrixDS<double> C(2.0, 2, 2);
        MatrixDS<double> C2(1.0, 2, 2);
        /**
         * 1 1    2 2 2
         * 1 1    2 2 2
         * 1 1
         */
        double v = 2.0;
        auto D   = A * v;
        D.assertSize(A);
        for(size_t i = 0; i < D.rows(); ++i) {
            for(size_t j = 0; j < D.columns(); ++j) { assert(D[i][j] == 2.0); }
        }
        //    B * C; <- not possible
        C* C2;

        MatrixDS<double> C3;
        C3 = C * C2;
        C3.assertSize(C);
        for(size_t i = 0; i < C3.rows(); ++i) {
            for(size_t j = 0; j < C3.columns(); ++j) { assert(C3[i][j] == 4.0); }
        }
        auto E = A * B;
        std::cout << E;
        assert(E.columns() == 3);
        assert(E.rows() == 3);
        for(size_t i = 0; i < E.rows(); ++i) {
            for(size_t j = 0; j < E.columns(); ++j) { assert(E[i][j] == 4); }
        }

        MatrixDS<double> vec(1.0, 10, 1);
        MatrixDS<double> vec2(2.0, 1, 10);

        auto resVec = vec * vec2;
        assert(resVec.rows() == 10);
        assert(resVec.columns() == 10);
        auto resVec2 = vec2 * vec;
        assert(resVec2.rows() == 1);
        assert(resVec2.columns() == 1);

        MatrixDS<double> vec2A(2.0, 2, 1);
        MatrixDS<double> mat2(1.0, 2, 2);
        MatrixDS<double> vec2b(4.0, 2, 1);
        MatrixDS<double> resA = mat2 * vec2A;

        assert(resA == vec2b);
        return true;
    }

    bool TestMatrixAddition() {
        MatrixDS<double> A(2.0, 2, 2);
        MatrixDS<double> B(2.0, 2, 2);
        MatrixDS<double> C(0, 2, 2);
        auto D = A + B;

        MatrixDS<double> E(4.0, 2, 2);
        assert(D == E);
        C += A;
        assert(C == B);

        MatrixDS<double> vec2(1.0, 2, 1);
        MatrixDS<double> vec2b(1.0, 2, 1);
        MatrixDS<double> vec2c(2.0, 2, 1);

        assert((vec2 + vec2b) == vec2c);

        // Impossible
        //    vec2 + A;
        return true;
    }

    bool TestHadamardMultiplication() {
        MatrixDS<double> A(1.0, 2, 2);
        MatrixDS<double> B(2.015, 2, 2);

        auto C = A.HadamardMulti(B);

        assert(C == B);

        MatrixDS<double> D({ { 0, 1 }, { 0, 0 } });
        MatrixDS<double> D2({ { 0, 1 }, { 0, 0 } });
        MatrixDS<double> E(5.0, 2, 2);
        MatrixDS<double> resultA({ { 0, 5.0 }, { 0, 0 } });
        MatrixDS<double> resultB({ { 0, 25.0 }, { 0, 0 } });

        assert(D.HadamardMulti(E) == resultA);
        D.HadamardMulti(E);
        assert(D == resultB);

        auto resD2E = HadamardMulti(D2, E);
        assert(resD2E == resultA);

        MatrixDS<double> Q1(10.0, 2, 5);
        MatrixDS<double> Q2(10.0, 3, 5);
        MatrixDS<double> Q3(10.0, 5, 2);

        // Impossible
        //    Q1.HadamardMulti(Q2);
        //    Q1.HadamardMulti(Q3);
        //    HadamardMulti(Q1, Q2);
        return true;
    }

    bool TestKroneckerMultiplication() {
        MatrixDS<double> A(1.0, 1, 2);
        MatrixDS<double> B(2.0, 2, 1);
        MatrixDS<double> C(2.0, 2, 2);

        auto resA = A.KroneckerMulti(B);

        assert(resA == C);

        MatrixDS<double> A2({ { 1, 2 }, { 3, 4 } });
        MatrixDS<double> B2({ { 0, 5 }, { 6, 7 } });
        MatrixDS<double> C2({ { 0, 5, 0, 10 }, { 6, 7, 12, 14 }, { 0, 15, 0, 20 }, { 18, 21, 24, 28 } });

        MatrixDS<double> resB = A2.KroneckerMulti(B2);

        assert(resB == C2);

        MatrixDS<double> A3({ { 1, 2 }, { 3, 4 }, { 5, 6 } });
        MatrixDS<double> A32({ { 1, 2 }, { 3, 4 }, { 5, 6 } });
        MatrixDS<double> B3({ { 7, 8 }, { 9, 0 } });
        MatrixDS<double> C3({ { 7, 8, 14, 16 },
                              { 9, 0, 18, 0 },
                              { 21, 24, 28, 32 },
                              { 27, 0, 36, 0 },
                              { 35, 40, 42, 48 },
                              { 45, 0, 54, 0 } });

        auto resC = A3.KroneckerMulti(B3);
        assert(resC == C3);

        auto resC2 = KroneckerMulti(A32, B3);
        assert(resC2 == resC && resC2 == C3);

        return true;
    }

    bool TestHorizontalConcat() {
        MatrixDS<double> A(1.0, 2, 2);
        MatrixDS<double> A2(1.0, 2, 2);
        MatrixDS<double> B(2.0, 2, 1);
        MatrixDS<double> C({ { 1.0, 1.0, 2.0 }, { 1.0, 1.0, 2.0 } });

        auto res1 = A.HorizontalConcat(B);

        assert(res1 == C);

        auto res2 = HorizontalConcat(A2, B);

        assert(res2 == C);

        return true;
    }

    bool TestMatrixDSCompare() {
        MatrixDS<double> A(2.0, 2, 2);
        MatrixDS<double> B(2.0, 2, 2);
        MatrixDS<double> C(0, 2, 2);
        MatrixDS<double> D(2.0, 2, 2);
        MatrixDS<double> E(2.0, 2, 2);
        MatrixDS<double> F({ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } });
        MatrixDS<double> G({ { 1, 0, 0 }, { 0, 1, 1 }, { 0, 0, 1 } });

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
        MatrixDS<double> A({ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } });
        MatrixDS<double> B({ { -1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } });
        MatrixDS<double> C({ { 3, 5, 2 }, { 8, 4, 8 }, { 2, 4, 7 } });
        MatrixDS<double> D({ { 9, 5, 2, 5 }, { 9, 5, 3, 7 }, { 6, 5, 4, 8 }, { 1, 5, 3, 7 } });
        MatrixDS<double> E({ { 1, 2 }, { 3, 4 } });
        assert(A.Determinant() == 1.0);
        assert(B.Determinant() == -1.0);
        assert(C.Determinant() == -164);
        assert(D.Determinant() == -40);
        assert(E.Determinant() == -2);
        return true;
    }

    bool TestMatrixDSTranspose() {
        // Vector
        MatrixDS<double> vec({ { 1, 2, 3 } });
        MatrixDS<double> vec2 = vec.Transpose();
        assert(vec2.columns() == vec.rows());
        assert(vec2.rows() == vec.columns());
        for(size_t i = 0; i < 3; ++i) { assert(vec[0][i] == vec2[i][0]); }
        // Matrix
        MatrixDS<double> A(1, 2, 2);
        assert(A == A.Transpose());
        MatrixDS<double> B({ { 1, 1 }, { 2, 2 } });
        MatrixDS<double> C({ { 1, 2 }, { 1, 2 } });

        assert(B.Transpose() == C);

        MatrixDS<double> D({ { 1, 1 }, { 2, 2 }, { 3, 3 } });
        MatrixDS<double> E({ { 1, 2, 3 }, { 1, 2, 3 } });
        assert(D.Transpose() == E);
        return true;
    }

    bool TestMatrixDSInit() {
        MatrixDS<float> matrix;
        matrix.Resize(2, 2);
        assert(matrix.rows() == 2);
        assert(matrix.columns() == 2);

        MatrixDS<double> m2({ { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } });
        assert(m2.rows() == 2);
        assert(m2.columns() == 11);

        MatrixDS<double> m3({ { 1 }, { 1 } });
        assert(m3.rows() == 2);
        assert(m3.columns() == 1);

        MatrixDS<double> m5({ { 1.0, 1.0 }, { 1.0, 1.0 } });
        for(size_t i = 0; i < 2; i++) {
            for(size_t j = 0; j < 2; j++) { assert(m5[i][j] == 1.0); }
        }
        MatrixDS<double> m6(1.0, 2, 2);
        for(size_t i = 0; i < 2; i++) {
            for(size_t j = 0; j < 2; j++) { assert(m6[i][j] == 1.0); }
        }
        return true;
    }

    bool TestWhere() {
        MatrixDS<double> A({ { 0, 0 }, { 1, 1 }, { 0, 0 } });
        std::function<bool(double)> cond = [](double i) { return bool(i == 1); };
        auto B = where(cond, A, { { { 1, 1 }, { 1, 1 }, { 1, 1 } } }, { { { 0, 0 }, { 0, 0 }, { 0, 0 } } });
        std::cout << B;
        assert(B == A);
        return true;
    }

public:
    void run(bool _extended) override {
        TestMatrixDSInit();
        TestMatrixDSMultiplication();
        TestMatrixAddition();
        TestMatrixDSCompare();
        TestMatrixDSDeterminant();
        TestMatrixDSTranspose();
        TestHadamardMultiplication();
        TestKroneckerMultiplication();
        TestHorizontalConcat();
        TestWhere();
    }
};

int main() {
    MatrixDSTestCase().run(false);
    return 0;
}
