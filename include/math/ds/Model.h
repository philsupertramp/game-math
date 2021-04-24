#pragma once

class ModelMetrics
{

};
class TrainingData
{

};
namespace LossFunctions {

    Matrix<double> Identity(const Matrix<double>& a){return a;}
}

class Model
{
public:
    std::function<Matrix<double>(const Matrix<double>&)> loss = LossFunctions::Identity;
    ModelMetrics modelMetrics;

    virtual void Compile() = 0;
    [[maybe_unused]] virtual TrainingData Train(const Tensor<double>& training, const Tensor<double>& validation = {}, const size_t& epochs = 1) = 0;
    virtual void Summary() const = 0;
    virtual Tensor<double> Predict(const Tensor<double>& X) = 0;
    void SetMetrics(const ModelMetrics& metrics){modelMetrics = metrics;}
    void SetLossFunction(const std::function<Matrix<double>(const Matrix<double>&)>& fun){loss = fun;}

};