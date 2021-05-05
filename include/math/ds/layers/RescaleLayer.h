#pragma once

#include "../../Tensor.h"
#include "Layer.h"


namespace Preprocessing {
    class RescaleLayer : public Layer<double>
    {
        double factor = 1.0;

    public:
        explicit RescaleLayer(double scalingFactor)
            : factor(scalingFactor) {        this->Type = "rescaling";
        }

        virtual void Replace(Tensor<double>& input) const override { input = input * factor; }


        virtual void Summary() override {
            std::cout << this->name << "\t\t\t\t\t(any)\t\t\t\t\t" << "0" << "\n"
            "_________________________________________________________________\n";
        }

    };
} // namespace Preprocessing