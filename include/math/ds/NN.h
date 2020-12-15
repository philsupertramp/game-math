#pragma once
#include <stdlib.h>
#include "MatrixDS.h"
#include "../format.h"
#include "any.h"
#include <ranges>

double sigmoid(const double &in){return 1.0/(1.0+exp(-in));}
double sigmoidDx(const double &in){return in * (1.0-in);}

MatrixDS<double> Activate(const MatrixDS<double> &in){
    auto out = new MatrixDS<double>(0, in.rows(), in.columns());
    for(size_t i = 0; i < out->rows(); i++){
        for(size_t j = 0; j < out->columns(); j++){
            (*out)[i][j] = sigmoid(in[i][j]);
        }
    }
    return *out;
}
MatrixDS<double> ActivateDerivative(const MatrixDS<double> &in){
    auto out = new MatrixDS<double>(0, in.rows(), in.columns());
    for(size_t i = 0; i < out->rows(); i++){
        for(size_t j = 0; j < out->columns(); j++){
            (*out)[i][j] = sigmoidDx(in[i][j]);
        }
    }
    return *out;
}

class NNLayer
{
public:
    NNLayer(size_t in, size_t out): in(in), out(out){
        weights = MatrixDS<double>::Random(in, out);
        bias = MatrixDS<double>::Random(out, 1);
    }
    NNLayer() = default;
    NNLayer(const NNLayer& other) = default;

    [[nodiscard]] MatrixDS<double>
    Evaluate(const MatrixDS<double>& input) const {
        return Activate(HorizontalConcat(weights.Transpose(), bias) * input);
    }

    [[nodiscard]] constexpr size_t Prev() const { return in; }
    [[nodiscard]] constexpr size_t Next() const { return out; }

    size_t in = 0;
    size_t out = 0;

    MatrixDS<double> weights;
    MatrixDS<double> bias;
};

struct DataSet{
    MatrixDS<double> Input;
    MatrixDS<double> Output;
};

template<size_t FeatureCount, size_t OutputCount>
class NN
{

public:
    NN(size_t numNeurons = 2){
        numLayers = 1;
        weights = MatrixDS<double>::Random(FeatureCount, numNeurons);
        bias = MatrixDS<double>::Random(numNeurons, 1);
        layers.push_back(MatrixDS<double>::Random(FeatureCount, numNeurons));
    }

    MatrixDS<double> FeedForward(const DataSet& ds){
        auto layer = MatrixDS<double>(0, ds.Input.rows(), weights.columns());
        for(size_t i = 0; i < layers.size(); i++){
            auto newLayer = Activate(ds.Input * weights);
            layers[i] = newLayer;
            layer = layer + newLayer;
        }

        MatrixDS<double> out;
        if(bias.columns() == 1 && bias.rows() == 1){
            out = Activate(layer * bias[0][0]);
        } else {
            out = Activate(layer * bias);
        }

        return out;
    }

    void BackPropagate(const DataSet& dataSet, const MatrixDS<double>& out, double eta = 0.5){
         for(size_t i = 0; i < layers.size(); i++){
             auto& layer = layers[i];
             auto scaledError = (dataSet.Output - out) * eta;
             auto d_bias = (layer.Transpose() *  HadamardMulti(scaledError, ActivateDerivative(out)));
             auto d_weights = dataSet.Input.Transpose() * HadamardMulti(HadamardMulti(scaledError, ActivateDerivative(out)) * bias.Transpose(), ActivateDerivative(layer));

             weights = weights + d_weights;
             bias = bias + d_bias;
         }
    }

    void Train(int maxEpoch = 10000){
        DataSet training{
            MatrixDS<double>({
                {0.0,   0.0,   1.0},
                {0.0,   1.0,   1.0},
                {1.0,   0.0, 1.0 },
                { 1.0,  1.0, 1.0 }
            }),
            MatrixDS<double>({
                             {0},
                             {1},
                             {1},
                             {0}
            })
        };
        double stopThreshold = 0.001;
        MatrixDS<double> currentOut(0, 4, 1);
        for(int i = 0; i < maxEpoch; ++i) {
            currentOut = FeedForward(training);
            BackPropagate(training, currentOut);

            MatrixDS<double>currentError = training.Output - currentOut;
            // Calculate loss
            if(i % 30 == 0) {
                auto loss = (HadamardMulti(currentError, currentError).sumElements()) / (double)currentError.rows();

                std::cout << format("Current loss: %.8f", loss) << std::endl;
                std::cout << "Current prediction: " << currentOut << std::endl;
                std::cout << "Actual output: " << training.Output << std::endl;
                if(loss < stopThreshold) {
                    std::cout<<"Threshold met at " << i << " iterations\n";
                    break;
                }
            }
        }
    }

    /**
     * Keep in mind, prev and next need to match in and output!
     * @tparam Prev
     * @tparam Next
     */
    void AddLayer(){
        layers.push_back(MatrixDS<double>(0,weights.rows(),weights.columns()));
        numLayers++;
    }
    void SetLayers(size_t numNewLayers){
        if(numLayers != numNewLayers){
            layers.clear();
            for(size_t i = 0; i < numNewLayers; i++){
                layers.push_back(MatrixDS<double>(0, weights.rows(),weights.columns()));
            }
        }
    }

    size_t numLayers;
    std::vector<MatrixDS<double>> layers;
    MatrixDS<double> weights;
    MatrixDS<double> bias;
};
