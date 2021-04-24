#pragma once

#include "Layer.h"
#include <math/ds/SGD.h>


enum ActivationFunctions { RELU = 0, LINEAR = 1, SIGMOID = 2 };

/**
 * **Caution!** can only process 1D-tensors
 * @tparam T tensor element type
 */
template<typename T>
class DenseLayer : public Layer<T>
{
    size_t numFeatures = 0;
    ActivationFunctions activationFunction;


    double eta; // Learning rate
    int n_iter; // number epochs
    bool w_initialized = false;

public:
    Matrix<double> weights; // Vector holding weights
    Matrix<double> costs;   // Vector holding classification error per epoch

    DenseLayer(size_t units, ActivationFunctions activation = ActivationFunctions::LINEAR)
        : Layer<T>()
        , numFeatures(units)
        , activationFunction(activation)
    {
        this->Type = "dense";
        auto weightDim = units + 1; // +1 for bias
        weights       = Matrix<double>::Random(weightDim, 1);
    }

    virtual void Replace([[maybe_unused]] Tensor<T>& input) const {
        assert(input.dim == 1 && input.maxDim == 1);
        auto activatedInput = input[0];
        switch(activationFunction) {
            case ActivationFunctions::LINEAR:{/* nothing to do */} break;
            case ActivationFunctions::RELU:{activatedInput.Apply(relu);} break;
            case ActivationFunctions::SIGMOID:{activatedInput.Apply(sigmoid);} break;
        }
        input.Set({SGD::netInput(activatedInput, weights)});
    }

    virtual void Summary() override {
        std::cout << this->name << "\t\t\t\t\t\t(None, " << weights.rows() << ", " << weights.columns()
                  << ", " << weights.elements() << ")\t\t\t\t\t" << "" /* */ << "\n"
            "_________________________________________________________________\n";
    }

};
