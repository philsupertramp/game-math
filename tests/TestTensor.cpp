#include "Test.h"
#include <math/Tensor.h>

class TensorTestCase : public Test
{
    bool TestTensor() {
        Tensor<double> t(2, 3);
        AssertEqual(t.maxDim, 2);
        AssertEqual(t.dim, 0);
        Matrix<double> A(1, 2, 2, 3);
        t.push_back(A);
        AssertEqual(t.dim, 1);
        AssertEqual(A, t[0]);

        AssertEqual(t.sum(), 12.0);

        // A * A
        auto B   = A * A;
        auto res = t * t;
        AssertEqual((res).sum(), B.sumElements());

        // A * A == 2.0 * A
        auto C = 2.0 * t;
        auto D = t * 2.0;
        AssertEqual(C.dim, D.dim);
        AssertEqual(C.sum(), B.sumElements());
        AssertEqual(D.sum(), C.sum());

        // extra push_back is gracefully ignored, just prints some message
        AssertEqual(t.dim, 1);
        t.push_back(C[0]);
        AssertEqual(t.dim, t.maxDim);
        t.push_back(C[0]);
        AssertEqual(t.dim, t.maxDim);


        return true;
    }

public:
    void run() override { TestTensor(); }
};

int main() {
    TensorTestCase().run();
    return 0;
}