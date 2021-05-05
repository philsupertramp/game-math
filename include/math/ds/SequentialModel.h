#pragma once

#include "layers/Layer.h"
#include "Model.h"
#include "utils.h"

template<typename T>
class SequentialModel
: public Model
{
    std::vector<std::shared_ptr<Layer<T>>> layers;

public:
    explicit SequentialModel(const std::initializer_list<std::shared_ptr<Layer<T>>>& initLayers)
        : layers(initLayers) { }

    virtual void Compile() override {
        size_t i = 0;
        for(auto layer : layers){
            layer->SetName(format("%s_%d", layer->Type.c_str(), i++));
        }
    }
    virtual TrainingData Train(const Tensor<double>& training, const Tensor<double>& validation = {}, const size_t& epochs = 1){
        // TODO:
        return {};
    }

    virtual Tensor<double> Predict(const Tensor<double>& X) {
        auto out = X;
        for(size_t i = 0; i < layers.size(); ++i){
            layers[i]->Replace(out);
        }
        return out;
    }

    virtual void Summary() const override {
        std::cout << ""
                     "Model: \"sequential\"\n"
                     "_________________________________________________________________\n"
                     "Layer (type)\t\t\t\tOutput Shape\t\t\tParam #   \n"
                     "=================================================================\n";
        for(auto layer : layers) {
            layer->Summary();
        }
    }
};
