#pragma once
#include "../format.h"
#include "MatrixDS.h"
#include "DataSet.h"
#include <cstdlib>
#include <fstream>
#include <ranges>
#include <sstream>

double sigmoid(const double& in) { return 1.0 / (1.0 + exp(-in)); }
double sigmoidDx(const double& in) { return in * (1.0 - in); }

MatrixDS<double> Activate(const MatrixDS<double>& in) {
    auto out = new MatrixDS<double>(0, in.rows(), in.columns());
    for(size_t i = 0; i < out->rows(); i++) {
        for(size_t j = 0; j < out->columns(); j++) { (*out)[i][j] = sigmoid(in[i][j]); }
    }
    return *out;
}
MatrixDS<double> ActivateDerivative(const MatrixDS<double>& in) {
    auto out = new MatrixDS<double>(0, in.rows(), in.columns());
    for(size_t i = 0; i < out->rows(); i++) {
        for(size_t j = 0; j < out->columns(); j++) { (*out)[i][j] = sigmoidDx(in[i][j]); }
    }
    return *out;
}

class NNLayer
{
public:
    NNLayer(size_t in, size_t out)
        : in(in)
        , out(out) {
        weights = MatrixDS<double>::Random(in, out);
        bias    = MatrixDS<double>::Random(out, 1);
    }
    NNLayer()                     = default;
    NNLayer(const NNLayer& other) = default;

    [[nodiscard]] MatrixDS<double> Evaluate(const MatrixDS<double>& input) const {
        return Activate(HorizontalConcat(weights.Transpose(), bias) * input);
    }

    [[nodiscard]] constexpr size_t Prev() const { return in; }
    [[nodiscard]] constexpr size_t Next() const { return out; }

    size_t in  = 0;
    size_t out = 0;

    MatrixDS<double> weights;
    MatrixDS<double> bias;
};


template<size_t FeatureCount, size_t OutputCount>
class NN
{
public:
    explicit NN(size_t numNeurons = 2) {
        numLayers = 1;
        weights   = MatrixDS<double>::Random(FeatureCount, numNeurons);
        bias      = MatrixDS<double>::Random(numNeurons, OutputCount);
        layers.push_back(MatrixDS<double>::Random(FeatureCount, numNeurons));
    }

    MatrixDS<double> FeedForward(const Set& ds) {
        auto layer = MatrixDS<double>(0, ds.Input.rows(), weights.columns());
        for(size_t i = 0; i < layers.size(); i++) {
            auto newLayer = Activate(ds.Input * weights);
            layers[i]     = newLayer;
            layer         = layer + newLayer;
        }

        MatrixDS<double> out;
        if(bias.columns() == 1 && bias.rows() == 1) {
            out = Activate(layer * bias[0][0]);
        } else {
            out = Activate(layer * bias);
        }

        return out;
    }

    void BackPropagate(const Set& dataSet, const MatrixDS<double>& out, double eta = 0.0051) {
        for(auto& layer : layers) {
            auto scaledError = (dataSet.Output - out) * eta;
            auto d_bias      = (layer.Transpose() * HadamardMulti(scaledError, ActivateDerivative(out)));
            auto d_weights =
            dataSet.Input.Transpose()
            * HadamardMulti(
            HadamardMulti(scaledError, ActivateDerivative(out)) * bias.Transpose(), ActivateDerivative(layer));

            weights = weights + d_weights;
            bias    = bias + d_bias;
        }
    }

    /**
     *
     * @param ds
     * @param maxEpoch
     * @param stopThreshold
     * @param eta
     * @param batchSize [ == -1 || > 0]
     */
    void Train(const DataSet& ds) {
        MatrixDS<double> currentOut(0, ds.InputCount, ds.OutputCount);
        bool firstHit = false;
        for(int i = 0; i < ds.maxEpoch; ++i) {
            auto batch = ds.Training.GetBatch(ds.batchSize);
            currentOut = FeedForward(batch);
            BackPropagate(batch, currentOut, ds.eta);

            // Calculate loss
            if(i % 30 == 0 || i == ds.maxEpoch - 1) {
                MatrixDS<double> currentError = batch.Output - currentOut;
                auto loss = (HadamardMulti(currentError, currentError).sumElements()) / (double)currentError.rows();

                std::cout << format("Current loss: %.8f", loss) << std::endl;
                std::cout << "Current prediction: " << currentOut << std::endl;
                std::cout << "Actual output: " << batch.Output << std::endl;
                if(loss < ds.stopThreshold) {
                    if(!firstHit){
                        firstHit = true;
                        continue;
                    }
                    std::cout << "Threshold met at " << i << " iterations\n";
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
    void AddLayer() {
        layers.push_back(MatrixDS<double>(0, weights.rows(), weights.columns()));
        numLayers++;
    }
    /**
     * sets #numNewLayers identical sequential layers
     * @param numNewLayers
     */
    void SetLayers(size_t numNewLayers) {
        if(numLayers != numNewLayers) {
            layers.clear();
            for(size_t i = 0; i < numNewLayers; i++) {
                layers.push_back(MatrixDS<double>(0, weights.rows(), weights.columns()));
            }
        }
    }

    size_t numLayers;
    std::vector<MatrixDS<double>> layers;
    MatrixDS<double> weights;
    MatrixDS<double> bias;
};
