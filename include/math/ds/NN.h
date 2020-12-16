#pragma once
#include "../format.h"
#include "MatrixDS.h"
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

struct DataSet {
    MatrixDS<double> Input;
    MatrixDS<double> Output;
    size_t InputCount;
    size_t OutputCount;
    size_t count = 0;

    DataSet(size_t inputCount, size_t outputCount) {
        InputCount  = inputCount;
        OutputCount = outputCount;
    }

    DataSet(const char* fileName, size_t inputCount, size_t outputCount) {
        InputCount  = inputCount;
        OutputCount = outputCount;
        ReadFromFile(fileName);
    }

    void ReadFromFile(const char* fileName) {
        std::string line;
        size_t lineCount = 0;

        std::ifstream dataFile2(fileName);
        if(dataFile2.is_open()) {
            while(getline(dataFile2, line, '\n')) { lineCount++; }
            dataFile2.close();
        }
        std::ifstream dataFile(fileName);
        count = 0;
        Input        = MatrixDS<double>(0, lineCount, InputCount);
        Output       = MatrixDS<double>(0, lineCount, OutputCount);
        if(dataFile.is_open()) {
            line = "";
            while(getline(dataFile, line, '\n')) {
                std::string val;
                std::stringstream lineStream(line);
                for(size_t i = 0; i < InputCount + OutputCount; i++) {
                    if(i < InputCount + OutputCount - 1) {
                        std::getline(lineStream, val, '\t');
                    } else {
                        std::getline(lineStream, val);
                    }
                    if(i < InputCount) {
                        Input[count][i] = std::atof(val.c_str());
                    } else {
                        Output[count][i - InputCount] = std::atoi(val.c_str());
                    }
                }
                ++count;
            }
            dataFile.close();
            //            Classes = OutputToClass(Output);
        } else {
            std::cerr << "Unable to open file";
        }
    }

    [[nodiscard]] DataSet GetBatch(int batchSize) const {
        if(batchSize == -1 || batchSize == (int)count){
            return *this;
        }

        auto newDS = DataSet(InputCount, OutputCount);
        newDS.Input = MatrixDS<double>(0, batchSize, InputCount);
        newDS.Output = MatrixDS<double>(0, batchSize, OutputCount);
        for(size_t i = 0; i < batchSize; i++){
            int index = (int)(Random::Get() * count);
        }

    }
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

    MatrixDS<double> FeedForward(const DataSet& ds) {
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

    void BackPropagate(const DataSet& dataSet, const MatrixDS<double>& out, double eta = 0.0051) {
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
    void Train(const DataSet& ds, int maxEpoch = 1000, double stopThreshold = 0.001, double eta = 0.0051, int batchSize = -1) {
        MatrixDS<double> currentOut(0, ds.InputCount, ds.OutputCount);
        for(int i = 0; i < maxEpoch; ++i) {
            auto batch = ds.GetBatch(batchSize);
            currentOut = FeedForward(batch);
            BackPropagate(batch, currentOut, eta);

            MatrixDS<double> currentError = batch.Output - currentOut;
            // Calculate loss
            if(i % 30 == 0) {
                auto loss = (HadamardMulti(currentError, currentError).sumElements()) / (double)currentError.rows();

                std::cout << format("Current loss: %.8f", loss) << std::endl;
                std::cout << "Current prediction: " << currentOut << std::endl;
                std::cout << "Actual output: " << batch.Output << std::endl;
                if(loss < stopThreshold) {
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
