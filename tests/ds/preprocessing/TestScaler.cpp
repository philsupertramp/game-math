#include "../../Test.h"
#include <math/ds/preprocessing/Scaler.h>

class StandardScalerTestCase : public Test
{
  bool TestConstructor() {
    auto clf = StandardScaler();
    return true;
  }

  bool TestFit() {
    Matrix<double> input_data(
    { { 2, 1 }, { 4, 1 }, { 2, 3 }, { 4, 3 }, { 8, 6 }, { 10, 8 }, { 2, 12 }, { 2, 14 }, { 4, 14 } });
    Matrix<double> labels({ { 0 }, { 0 }, { 0 }, { 0 }, { 1 }, { 1 }, { 2 }, { 2 }, { 2 } });
    Matrix<double> expected_weights = { { 38.0 / 9.0, 62.0 / 9.0 } };

    auto clf = StandardScaler();
    clf.fit(input_data, labels);

    AssertEqual(clf.means, expected_weights);

    clf = StandardScaler();
    clf.fit(input_data, labels);

    AssertEqual(clf.means, expected_weights);

    return true;
  }

  bool TestTransform() {
    Matrix<double> input_data(
    { { 2, 1 }, { 4, 1 }, { 2, 3 }, { 4, 3 }, { 8, 6 }, { 10, 8 }, { 2, 12 }, { 2, 14 }, { 4, 14 } });
    Matrix<double> labels({ { 0 }, { 0 }, { 0 }, { 0 }, { 1 }, { 1 }, { 2 }, { 2 }, { 2 } });
    Matrix<double> test_data({ { 1, 0 }, { 8, 8 }, { 0, 1 }, { 3, 16 } });
    Matrix<double> expected_labels({ { 0 }, { 1 }, { 0 }, { 2 } });

    auto clf = StandardScaler();
    clf.fit(input_data, labels);

    auto transformions = clf.transform(test_data);
    AssertEqual(transformions, expected_labels);

    return true;
  }


public:
  virtual void run() {
    TestConstructor();
    TestFit();
    TestTransform();
  }
};

int main() {
  StandardScalerTestCase().run();
  return 0;
}
