#pragma once
#include "Classifier.h"
#include "../matrix_utils.h"
#include <cstddef>
#include <limits>
#include <vector>
#include <bits/stdc++.h>


class ActivationFunction
{
public:
  virtual Matrix<double> operator()(const Matrix<double>& x) { return Matrix<double>();}
  virtual Matrix<double> grad(const Matrix<double>& x) { return Matrix<double>(); }
};
class LossFunction
{
public:
  virtual double operator()(const Matrix<double>& output, const Matrix<double>& target) { return std::numeric_limits<double>::max(); }
  virtual Matrix<double> grad(const Matrix<double>& output, const Matrix<double>& target) { return Matrix<double>();}
};

class Sigmoid
  : public ActivationFunction
{
public:
  Matrix<double> operator()(const Matrix<double>& x) {
    return 1./ ( 1.0 + exp(x));
  }

  Matrix<double> grad(const Matrix<double>& x) {
    return HadamardMulti(x, 1.0 - x);
  }
};

class MSE
  : public LossFunction
{
  public:

  double operator()(const Matrix<double>& output, const Matrix<double>& target) {
    return square(output - target).sumElements()/output.rows();
  }

  Matrix<double> grad(const Matrix<double>& output, const Matrix<double>& target) {
    return output - target;
  }
};

class MLP
  : public Classifier
{
  size_t num_classes;
  size_t epochs;
  double learn_rate;
  std::vector<std::pair<Matrix<double>,Matrix<double>>> layers;
  ActivationFunction* activation_function;
  LossFunction* loss_function;

public:
  MLP(const std::initializer_list<std::initializer_list<size_t>>& layer_sizes,
      size_t train_epochs, ActivationFunction* activation,
      LossFunction* loss, double learning_rate = 1e-3)
    : Classifier()
  {
    for(auto dims : layer_sizes){
      layers.push_back({Matrix<double>::Random((*dims.begin()), (*(dims.end()-1)), 1, -0.5, 0.5), Matrix<double>(0, (*dims.begin()), 1)});
    }
    num_classes = (*((*(layer_sizes.end()-1)).end()-2));
    activation_function = activation;
    loss_function = loss;
    epochs = train_epochs;
    learn_rate = learning_rate;
  }


  void fit(const Matrix<double> &X, const Matrix<double> &y) override {

    for(size_t epoch = 0; epoch < epochs; ++epoch){
      int correct = 0;
      double running_loss = 0;
      for(auto elem : zip(X, y)){
        auto xi = elem.first;
        auto yi = elem.second;

        auto out = xi.Transpose();
        std::vector<Matrix<double>> hidden_values;
        hidden_values.push_back(out);
        for(auto layer : layers){
          out = (*activation_function)(layer.second + (layer.first * out));
          hidden_values.push_back(out);
        }

        auto error = (*loss_function)(out, yi);

        running_loss += error;

        correct += int(argmax(out)==argmax(yi));

        backpropagate(out, hidden_values, yi);
      }
      std::cout << "EPOCH: " << epoch + 1 <<  "\t"<< "Accuracy: " << ((correct/(double)X.rows()) * 100.) << " Loss: " << (running_loss/(double)X.rows()) << std::endl;
    }
  }

  void backpropagate(const Matrix<double>& output, const std::vector<Matrix<double>>& hidden_values, const Matrix<double>& yi){
    auto grad_output = loss_function->grad(output, yi);
    for(int i = layers.size() - 1; i >= 0; --i){
      layers[i].first += -learn_rate * (grad_output * hidden_values[i].Transpose());
      layers[i].second += -learn_rate * grad_output;
      grad_output = hidden_grad(grad_output, hidden_values[i], i);
    }
  }

  Matrix<double> hidden_grad(const Matrix<double>& grad_output, const Matrix<double>& hidden, size_t layer){
    return HadamardMulti(layers[layer].first.Transpose() * grad_output, activation_function->grad(hidden));
  }


  Matrix<double> _predict(const Matrix<double> &x) {
    auto out = x.Transpose();
    if(!out.IsVector()){
      std::cerr << "Input is no vector, omg uwu =o.o=" << std::endl; 
    }
    for(auto layer : layers){
      out = (*activation_function)(layer.second + (layer.first * out));
    }
    return out;
  }

  Matrix<double> predict(const Matrix<double> &X) override {
    Matrix<double> out(0, X.rows(), num_classes);

    for(size_t idx = 0; idx < X.rows(); ++idx){
      out.SetSlice(idx, _predict(X.GetSlice(idx)));
    }
    return out;
  }

};
