#include "../Test.h"
#include <math/ds/MLP.h>
#include <math/ds/DataSet.h>
#include <math/ds/utils.h>
#include <math/ds/preprocessing/Encoder.h>


class MLPTestCase : public Test
{
  bool TestMLPIris() {
    Set dataset  = Set("./tests/ds/iris_dataset_train.tsv", 4, 1);
    Set test_set = Set("./tests/ds/iris_dataset_test.tsv", 4, 1);

    auto activation = Sigmoid();
    auto loss = MSE();
    auto mlp = MLP({{16, 4}, {16, 16}, {3, 16}}, 15, &activation, &loss);

    auto encoder = OneHotEncoder();
    encoder.fit(dataset.Output, Matrix<double>());
    auto train_output = encoder.transform(dataset.Output);
    auto test_output = encoder.transform(test_set.Output);


    mlp.fit(dataset.Input, train_output);

    auto preds = mlp.predict(test_set.Input);

    auto acc = accuracy(argmax(preds, 1), test_output);

    std::cout << "Iris flower classification, achieved " << acc * 100. << "% accuracy." << std::endl;
    //
    return true;
  }

public:
  void run() override {
    TestMLPIris();
  }
};

int main() {
  MLPTestCase().run();
  return 0;
}
