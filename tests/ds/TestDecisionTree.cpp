#include "../Test.h"
#include <math/ds/DataSet.h>
#include <math/ds/DecisionTree.h>
#include <math/ds/utils.h>


class DecisionTreeTestCase : public Test
{
  bool TestCountBins(){
    Matrix<double>A{{0,0,0,0,0,0,0,1,1,1}};
    Matrix<double>B{{0,0,0,0,0,1,1,1,1,1}};
    Matrix<double>C{{0,0,0,1,1,1,1,1,1,1}};

    auto bins_a = count_bins(A);
      std::cout << bins_a << std::endl;
    AssertEqual(bins_a, {{0,7},{1,3}});
    AssertEqual(count_bins(B), {{0,5},{1,5}});
    AssertEqual(count_bins(C), {{0,3},{1,7}});
    
    return true;
  }
  bool TestGini() {
    Matrix<double>A{{0,0,0,0,0,0,0,1,1,1}};
    Matrix<double>B{{0,0,0,0,0,1,1,1,1,1}};
    Matrix<double>C{{0,0,0,1,1,1,1,1,1,1}};

    //AssertEqual(gini(A), 0.42);
    AssertEqual(gini(B), 0.5);
    AssertEqual(gini(C), gini(A));
    return true;
  }
  bool TestConstructor() {
    auto clf = DecisionTree(1);
    DecisionTree(1, 10);
    DecisionTree(1, 10, ImpurityMeasure::GINI);
    return true;
  }

  bool TestFit() {
    Matrix<double> input_data(
    { { 2, 1 }, { 4, 1 }, { 2, 3 }, { 4, 3 }, { 8, 6 }, { 10, 8 }, { 2, 12 }, { 2, 14 }, { 4, 14 } });
    Matrix<double> labels({ { 0 }, { 0 }, { 0 }, { 0 }, { 1 }, { 1 }, { 2 }, { 2 }, { 2 } });

    auto clf = DecisionTree(2);
    clf.fit(input_data, labels);

    AssertTrue(clf.GetRootNode()->type == DecisionNodeType::DECISION);

    return true;
  }

  bool TestPredict() {
    Matrix<double> input_data(
    { { 2, 1 }, { 4, 1 }, { 2, 3 }, { 4, 3 }, { 8, 6 }, { 10, 8 }, { 2, 12 }, { 2, 14 }, { 4, 14 } });
    Matrix<double> labels({ { 0 }, { 0 }, { 0 }, { 0 }, { 1 }, { 1 }, { 2 }, { 2 }, { 2 } });
    Matrix<double> test_data({ { 1, 0 }, { 8, 8 }, { 0, 1 }, { 5, 16 } });
    Matrix<double> expected_labels({ { 0 }, { 1 }, { 0 }, { 2 } });

    auto clf = DecisionTree(2);
    clf.fit(input_data, labels);

    auto predictions = clf.predict(test_data);
    std::cout << predictions << std::endl;
    AssertEqual(predictions, expected_labels);

    return true;
  }

  /**
   * Usage example: Iris flower 3-class classification problem.
   *
   * Uses 2/3 train and 1/3 validation data. Data was shuffled prior to be written into
   * TSV files.
   */
  bool TestPredictionOnIrisData() {
    Set dataset  = Set("../../tests/ds/iris_dataset_train.tsv", 4, 1);
    Set test_set = Set("../../tests/ds/iris_dataset_test.tsv", 4, 1);

    auto clf = DecisionTree(2, 5);
    clf.fit(dataset.Input, dataset.Output);

    auto preds = clf.predict(test_set.Input);

    auto acc = accuracy(preds, test_set.Output);

    std::cout << clf << std::endl;
    std::cout << "Iris flower classification, achieved " << acc * 100. << "% accuracy." << std::endl;
    AssertEqual(acc, 1.0);
    return true;
  }


public:
  virtual void run() {
    TestCountBins();
    TestGini();
    TestConstructor();
    TestFit();
    TestPredict();
    TestPredictionOnIrisData();
  }
};

int main() {
  DecisionTreeTestCase().run();
  return 0;
}
