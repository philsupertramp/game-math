#include "../../Test.h"
#include <math/ds/layers/PaddingLayer.h>

class PaddingLayerTestCase : public Test
{
    bool TestProcess() {
        Matrix<double> A({ { 1 } });
        Matrix<double> B({ { 0, 0, 0 }, { 0, 1, 0 }, { 0, 0, 0 } });
        Matrix<double> C(
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } });
        Tensor<double> tensor(1, 1);
        tensor.push_back(A);
        auto layer  = PaddingLayer(0, 1);
        auto layer2 = PaddingLayer(0, 2);
        auto out    = layer.Process(tensor);
        auto out2   = layer2.Process(tensor);
        AssertEqual(out[0], B);
        AssertEqual(out2[0], C);

        return true;
    }

    bool TestReplace() {
        Matrix<double> A({ { 1 } });
        Matrix<double> B({ { 0, 0, 0 }, { 0, 1, 0 }, { 0, 0, 0 } });
        Tensor<double> tensor(1, 1);
        tensor.push_back(A);
        PaddingLayer(0, 1).Replace(tensor);
        AssertEqual(tensor[0], B);
        return true;
    }

public:
    void run() override {
        TestProcess();
        TestReplace();
    }
};

int main() {
    PaddingLayerTestCase().run();
    return 0;
}