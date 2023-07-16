#include "../Predictor.h"
#include "../../matrix_utils.h"


class OneHotEncoder : public Transformer
{
  Matrix<int> unique_values;

public:
  OneHotEncoder()
    : Transformer() { }

  void fit(const Matrix<double>& X, [[maybe_unused]] const Matrix<double>& y) override { unique_values = unique(X, 0); }

  Matrix<double> transform(const Matrix<double>& in) override {
    Matrix<double> out(0, in.rows(), unique_values.columns());
    for(size_t i = 0; i < in.rows(); i++){
      out(i, in(i, 0)) = 1;
    }
    return out; 
  }
};
