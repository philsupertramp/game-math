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

class Layer
{
public:
    Layer(size_t in, size_t out)
    : in(in)
    , out(out) 
    {
        weights   = MatrixDS<double>::Random(in, out);
        bias      = MatrixDS<double>::Random(out, 1);
        output = MatrixDS<double>(0, out, 1);
    }
    Layer()                     = default;
    Layer(const Layer& other) = default;

    [[nodiscard]] constexpr size_t In() const { return in; }
    [[nodiscard]] constexpr size_t Out() const { return out; }

    [[nodiscard]] MatrixDS<double> Evaluate(const MatrixDS<double>& input) const{
        auto a = (input * weights);
        for(size_t i = 0; i < a.columns(); i++){
            for(size_t j = 0; j < a.rows(); j++){
                a[j][i] += bias[i][0];
            }
        }
        return  a;
    }

    size_t in  = 0;
    size_t out = 0;

    MatrixDS<double> weights;
    MatrixDS<double> bias;
    MatrixDS<double> output;
};

class LinearLayer
: public Layer
{
public:
    LinearLayer(size_t inputCount, size_t numNeurons)
    : Layer(inputCount, numNeurons)
    {

    }
};

class LayerConfiguration
{
public:
    LayerConfiguration(std::initializer_list<Layer> layerConfig){
        validateLayerConfig(layerConfig);
        layers = layerConfig;
    }

    static void validateLayerConfig(std::initializer_list<Layer> layerConfig){
        assert(layerConfig.size() > 0);

        auto inputSize = layerConfig.begin()->in;
        for(const auto& layer : layerConfig){
            assert(layer.in == inputSize);
        }
    }

    inline std::vector<Layer>::iterator begin() noexcept { return layers.begin(); }
    [[nodiscard]] inline std::vector<Layer>::const_iterator cbegin() const noexcept { return layers.cbegin(); }
    inline std::vector<Layer>::iterator end() noexcept { return layers.end(); }
    [[nodiscard]] inline std::vector<Layer>::const_iterator cend() const noexcept { return layers.cend(); }

    inline std::vector<Layer>::reverse_iterator rbegin() noexcept { return layers.rbegin(); }
    [[nodiscard]] inline std::vector<Layer>::const_reverse_iterator crbegin() const noexcept { return layers.crbegin(); }
    inline std::vector<Layer>::reverse_iterator rend() noexcept { return layers.rend(); }
    [[nodiscard]] inline std::vector<Layer>::const_reverse_iterator crend() const noexcept { return layers.crend(); }

    Layer operator[](size_t index){
        return layers[index];
    }
    Layer operator[](size_t index) const {
        return layers[index];
    }

    [[nodiscard]] size_t size() const { return layers.size(); }

private:
    std::vector<Layer> layers;
};


template<size_t FeatureCount, size_t OutputCount>
class NN
{
public:
    explicit NN() {
        numLayers = 1;
    }

    MatrixDS<double> FeedForward(const Set& ds) {
        for(auto & layer : *layers) {
            layer.output = layer.Evaluate(ds.Input);
//            layer         = layer + newLayer;
        }
        return (*layers)[layers->size()-1].output;
    }

    void BackPropagate(const Set& dataSet, const MatrixDS<double>& out, double eta = 0.0051) {
        auto scaledError = (dataSet.Output - out) * eta;
        auto outputError = HadamardMulti(scaledError, ActivateDerivative(out));
        for(auto layer = layers->rbegin(); layer != layers->rend(); layer++) {
            auto d_bias      = (layer->output.Transpose() * outputError);
            auto d_weights = dataSet.Input.Transpose()
            * HadamardMulti(outputError * layer->bias.Transpose(), ActivateDerivative(layer->output));

            layer->weights = layer->weights + d_weights;
            layer->bias    = layer->bias + d_bias;
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
            if(i % 10 == 0 || i == ds.maxEpoch - 1) {
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

    void SetSequentialLayers(const LayerConfiguration& config) {
        layers = std::make_shared<LayerConfiguration>(config);
    }

    size_t numLayers = 0;
    std::shared_ptr<LayerConfiguration> layers;
};
