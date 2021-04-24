#include "../../Test.h"
#include <math/ds/layers/NormalizationLayer.h>

class NormalizationLayerTestCase : public Test
{
    bool TestProcess() {
        auto layer  = Preprocessing::NormalizationLayer<double>();
        auto layer2 = Preprocessing::NormalizationLayer<double>(false);

        Tensor<double> tensor(1, 1);
        Matrix<double> A({ { 0, 1, 2, 1 } });
        tensor.push_back(A);

        auto out = layer.Process(tensor);
        Matrix<double> B({ { 0, 0.5, 1, 0.5 } });
        std::cout << out[0];

        AssertEqual(out[0], B);
        AssertEqual(layer.Process(out)[0], B);

        Tensor<double> tensor3D(1, 4);
        Matrix<double> C({ { { 0, 1, 2, 3 } } });
        tensor3D.push_back(C);

        auto out2 = layer.Process(tensor3D);
        Matrix<double> D({ { { 0, 1.0 / 3, 2.0 / 3, 1 } } });

        AssertEqual(out2[0], D);

        Tensor<double> tensor1D(2, 1);
        Matrix<double> E({ { 0, 1, 2, 1 } });
        Matrix<double> F({ { 0, 1, 2, 3 } });
        tensor1D.push_back(E);
        tensor1D.push_back(F);

        auto out3 = layer.Process(tensor1D);
        Matrix<double> G({ { 0, 0.5, 1, 0.5 } });
        Matrix<double> H({ { 0, 1.0 / 3, 2.0 / 3, 1 } });

        AssertEqual(out3[0], G);
        AssertEqual(out3[1], H);

        auto out4 = layer2.Process(tensor1D);
        Matrix<double> I({ { 0, 1 / 3.0, 2 / 3.0, 1 / 3.0 } });
        Matrix<double> J({ { 0, 1.0 / 3, 2.0 / 3, 1 } });

        AssertEqual(out4[0], I);
        AssertEqual(out4[1], J);


        // This won't work
        //        auto err = Preprocessing::NormalizationLayer().Process(2);

        return true;
    }

    bool TestReplace() {
        Matrix<double> A({ { 0, 1, 2, 1 } });
        Matrix<double> B({ { 0, 0.5, 1, 0.5 } });
        Tensor<double> t(1, 1);
        t.push_back(A);
        Preprocessing::NormalizationLayer<double>().Replace(t);
        AssertEqual(t[0], B);
        return true;
    }

public:
    void run() override {
        TestProcess();
        TestReplace();
    }
};

int main() {
    NormalizationLayerTestCase().run();
    return 0;
}