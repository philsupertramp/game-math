#pragma once
#include "../format.h"
#include "DataSet.h"
#include "Layer.h"
#include "MatrixDS.h"
#include <cstdlib>
#include <fstream>
#include <ranges>
#include <sstream>


struct LayerStruct {
    size_t in;
    size_t out;
};

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

    void SGD(const DataSet& ds, int epochs, int mini_batch_size, double eta){
         auto training = ds.Training;
         auto test = ds.Test;
         size_t n_test = test.count;

         for(int j = 0; j<epochs; j++){
             // shuffle training data
             training.Shuffle();
             // build mini batches
             auto miniBatches = buildMiniBatches(training, mini_batch_size);
             // iterate through batch list
             for(auto& batch : miniBatches){
                 updateMiniBatch(batch, eta);
             }
             if(ds.verbose){
                 std::cout << format("Epoch %d : %d / %d", j, evaluate(test), n_test) << std::flush;
             }
         }
    }

    double evaluate(const Set& ds){
         double out = 0;
         for(size_t i = 0; i < ds.count; i++){
             out += (int)(argmax(feedforward(from_vptr(ds.Input[i], { 1, ds.Input.columns() }))) == ds.Output[i][0]);
         }
         return out;
    }

    MatrixDS<double> feedforward(const MatrixDS<double>& input){
        MatrixDS<double> out = input;
        for(const auto& layer : *layers){
            out = Activate((layer.weights.Transpose() * out)  + layer.bias);
        }
        return out;
    }

    void SetLayers(const LayerConfiguration& config) {
        layers = std::make_shared<LayerConfiguration>(config);
    }

    size_t numLayers = 0;
    std::shared_ptr<LayerConfiguration> layers;

private:
    void updateMiniBatch(Set& batch, double eta){
        auto nablaW = initDeltaW();
        auto nablaB = initDeltaB();
        for(size_t i = 0; i < batch.Input.rows(); i++){
            auto result = backprop(from_vptr(batch.Input[i], {1, batch.Input.columns()}), from_vptr(batch.Output[i], {1, batch.Output.columns()}));
            for(size_t j = 0; j < result.first.size(); j++){
                nablaB[j] = nablaB[j] + result.first[j];
                nablaW[j] = nablaW[j] + result.second[j];
            }
        }

        updateWeights(nablaW, eta, batch.count);
        updateBiases(nablaB, eta, batch.count);

    }
    std::pair<std::vector<MatrixDS<double>>,std::vector<MatrixDS<double>>> backprop(const MatrixDS<double>& input, const MatrixDS<double>& output){
        std::vector<MatrixDS<double>> nablaB;
        std::vector<MatrixDS<double>> nablaW;
        auto activation = input;
        std::vector<MatrixDS<double>> activations({input});
        std::vector<MatrixDS<double>> zs;
        size_t index = 0;
        for(const auto& layer : *layers){
            auto z = (layer.weights * activation) + layer.bias;
            zs.push_back(z);
            activation = Activate(z);
            activations.push_back(activation);

            index++;
        }
        auto delta = HadamardMulti(costDerivative(activations[index], output), ActivateDerivative(activations[index]));
        nablaB.push_back(delta);
        nablaW.push_back(delta * activations[index-1].Transpose());

        for(size_t l = 2; l <= layers->size(); l++){
            auto z = zs[zs.size()-l];
            auto sp = ActivateDerivative(z);
            delta = HadamardMulti(((*layers)[layers->size()-l+1].weights.Transpose() * delta), sp);
            nablaB.push_back(delta);
            nablaW.push_back(delta * activations[activations.size()-l-1].Transpose());
        }
        std::reverse(nablaB.begin(), nablaB.end());
        std::reverse(nablaW.begin(), nablaW.end());
        return std::make_pair(nablaB, nablaW);
    }
    static MatrixDS<double> costDerivative(const MatrixDS<double>& output_activations, const MatrixDS<double>& y){
        return output_activations - y;
    }
    void updateWeights(std::vector<MatrixDS<double>> nablaW, double eta, size_t batch_size){
        for(size_t i=0;i<nablaW.size(); i++){
            (*layers)[i].weights = (*layers)[i].weights-nablaW[i]*(eta/batch_size);
        }
    }
    void updateBiases(std::vector<MatrixDS<double>> nablaB, double eta, size_t batch_size){
        for(size_t i=0;i<nablaB.size(); i++){
            (*layers)[i].bias = (*layers)[i].bias-nablaB[i]*(eta/batch_size);
        }

    }
    std::vector<MatrixDS<double>> initDeltaW(){
        std::vector<MatrixDS<double>> out;
        size_t i = 0;
        for(const auto& layer : *layers){
            out.push_back(MatrixDS<double>(0, layer.weights.rows(), layer.weights.columns()));
            i++;
        }
        return out;
    }
    std::vector<MatrixDS<double>> initDeltaB(){
        std::vector<MatrixDS<double>> out;
        size_t i = 0;
        for(const auto& layer : *layers){
            out.push_back(MatrixDS<double>(0, layer.bias.rows(), layer.bias.columns()));
            i++;
        }
        return out;
    }
    static std::vector<Set> buildMiniBatches(const Set& set, int mini_batch_size){
        std::vector<Set> output(set.count / mini_batch_size + 1);
        for(size_t k = 0; k < set.count; k+=mini_batch_size){
            auto newDS = Set(set.InputCount, set.OutputCount);
            newDS.Input = MatrixDS<double>(0, mini_batch_size, set.InputCount);
            newDS.Output = MatrixDS<double>(0, mini_batch_size, set.OutputCount);
            for(int i = 0; i < mini_batch_size; i++){
                for(size_t in = 0; in < set.InputCount; in++){
                    newDS.Input[i][in] = set.Input[i + k][in];
                }
                for(size_t out = 0; out < set.OutputCount; out++){
                    newDS.Output[i][out] = set.Output[i + k][out];
                }
            }
            output[k / mini_batch_size] = newDS;
        }
        return output;
    }
};
