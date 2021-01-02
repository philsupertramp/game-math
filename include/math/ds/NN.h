#pragma once
#include "../format.h"
#include "DataSet.h"
#include "Layer.h"
#include "MatrixDS.h"
#include <cstdlib>
#include <fstream>
#include <ranges>
#include <sstream>


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
        /**
         *
        training_data = list(training_data)
        n = len(training_data)

        if test_data:
            test_data = list(test_data)
            n_test = len(test_data)

        for j in range(epochs):
            random.shuffle(training_data)
            mini_batches = [
                training_data[k:k+mini_batch_size]
                for k in range(0, n, mini_batch_size)]
            for mini_batch in mini_batches:
                self.update_mini_batch(mini_batch, eta)
            if test_data:
                print("Epoch {} : {} / {}".format(j,self.evaluate(test_data),n_test));
            else:
                print("Epoch {} complete".format(j))
         */
         auto training = ds.Training;
         auto test = ds.Test;
         size_t n_test = test.count;

         for(int j = 0; j<epochs; j++){
             // shuffle training data
             training.Shuffle();
             // build mini batches
             auto miniBatches = buildMiniBatches(training, mini_batch_size);
             // iterate through batch list
             std::vector<MatrixDS<double>> batches;
             for(auto& batch : batches){
                 updateMiniBatch(batch, eta);
             }
         }
    }

    void SetSequentialLayers(const LayerConfiguration& config) {
        layers = std::make_shared<LayerConfiguration>(config);
    }

    size_t numLayers = 0;
    std::shared_ptr<LayerConfiguration> layers;

private:
    void updateMiniBatch(MatrixDS<double>& batch, double eta){
        /**
         *
        nabla_b = [np.zeros(b.shape) for b in self.biases]
        nabla_w = [np.zeros(w.shape) for w in self.weights]
        for x, y in mini_batch:
            delta_nabla_b, delta_nabla_w = self.backprop(x, y)
            nabla_b = [nb+dnb for nb, dnb in zip(nabla_b, delta_nabla_b)]
            nabla_w = [nw+dnw for nw, dnw in zip(nabla_w, delta_nabla_w)]
        self.weights = [w-(eta/len(mini_batch))*nw
                        for w, nw in zip(self.weights, nabla_w)]
        self.biases = [b-(eta/len(mini_batch))*nb
                       for b, nb in zip(self.biases, nabla_b)]
         */

    }
    std::vector<MatrixDS<double>> buildMiniBatches(const Set& set, int mini_batch_size){
        //mini_batches = [
        //                training_data[k:k+mini_batch_size]
        //                for k in range(0, n, mini_batch_size)]
        std::vector<MatrixDS<double>> out;
        for(size_t k = 0; k < set.count; k+=mini_batch_size){
            MatrixDS<double> newElement;
            for(size_t elem = 0; elem < k+mini_batch_size; elem++){
                out[k]
            }
        }
    }
};
