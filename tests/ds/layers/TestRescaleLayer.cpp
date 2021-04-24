#include "../../Test.h"
#include <math/ds/layers/RescaleLayer.h>
#include <math/ds/utils.h>

class RescaleLayerTestCase : public Test
{
    bool TestProcess() {
        Matrix<double> A({ { 0, 1, 2, 1 } });
        Tensor<double> tensor(1, 1);
        tensor.push_back(A);
        Matrix<double> B({ { 0, 2, 4, 2 } });
        Tensor<double> tensor3D(1, 3);
        Matrix<double> E({ { { 0, 2, 4 }, { 2, 4, 2 } } });
        tensor3D.push_back(E);
        Matrix<double> F({ { { 0, 0.5, 1 }, { 0.5, 1, 0.5 } } });
        auto layer  = Preprocessing::RescaleLayer(2.0);
        auto layer2 = Preprocessing::RescaleLayer(0.25);
        auto out    = layer.Process(tensor);
        auto out2   = layer2.Process(tensor3D);
        AssertEqual(out[0], B);
        AssertEqual(out2[0], F);

        // element of upscaled tensor == upscaled element
        AssertEqual(layer.Process(tensor)[0], 2.0 * tensor[0]);

        return true;
    }

    bool TestReplace() {
        Tensor<double> A(1, 1);
        A.push_back(Matrix<double>({ { 0, 1, 2, 1 } }));
        Matrix<double> B({ { 0, 2, 4, 2 } });
        Tensor<Matrix<double>> C(1, 1);
        auto layer = Preprocessing::RescaleLayer(2.0);
        layer.Replace(A);
        AssertEqual(A[0], B);
        return true;
    }

public:
    void run() override {
        TestProcess();
        TestReplace();
    }
};

int main() {
    RescaleLayerTestCase().run();
    return 0;
}