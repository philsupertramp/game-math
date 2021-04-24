#include "../../Test.h"
#include <math/Tensor.h>
#include <math/ds/layers/FlattenLayer.h>


class FlattenLayerTestCase : public Test
{
    bool TestFlattenLayer() {
        auto layer = FlattenLayer<double>();
        Tensor<double> tensor(2, 3);
        Matrix<double> X(1, 2, 2, 3);
        tensor.push_back(X);
        layer.Replace(tensor);

        AssertEqual(tensor.dim, 1);
        AssertEqual(tensor.elementDim, 1);
        auto out = tensor[0];
        AssertEqual(out.rows(), X.rows() * X.columns() * X.elements());

        auto tensor2 = layer.Process(tensor);

        AssertEqual(tensor2.dim, 1);
        AssertEqual(tensor2.elementDim, 1);
        auto out2 = tensor2[0];
        AssertEqual(out2.rows(), out.rows());
        AssertEqual(out2.columns(), out.columns());
        AssertEqual(out2.elements(), out.elements());
        return true;
    }

public:
    void run() { TestFlattenLayer(); }
};

int main() {
    FlattenLayerTestCase().run();

    return 0;
}