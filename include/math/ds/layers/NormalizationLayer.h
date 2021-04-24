#pragma once

#include "../../Matrix.h"
#include "../../Tensor.h"
#include "Layer.h"
#include <math/ds/utils.h>

namespace Preprocessing {
    /**
     * Normalization layer normalizes given data according to the formula
     * normalizedX = (x-min(x))/(max(x)-min(x))
     * @tparam T
     */
    template<typename T>
    class NormalizationLayer : public Layer<T>
    {
        bool perDimension = true;

    public:
        NormalizationLayer(bool normalizePerDimension = true)
            : Layer<T>()
            , perDimension(normalizePerDimension) {         this->Type = "normalization";
        }
        /**
         * In-place normalization of a given tensor
         * normalization
         * @param input matrix to normalize
         */
        virtual inline void Replace(Tensor<T>& input) const override {
            if(perDimension) {
                for(size_t dim = 0; dim < input.dim; ++dim) {
                    double minVal = min(input[dim]);
                    double maxVal = max(input[dim]);
                    input[dim] -= Matrix<double>(minVal, input[dim].rows(), input[dim].columns(), input[dim].elements());
                    input[dim] *= (1.0 / (maxVal - minVal));
                }
                return;
            } else {
                double minVal = min(input);
                double maxVal = max(input);
                for(size_t dim = 0; dim < input.dim; ++dim) {
                    input[dim] -= Matrix<double>(minVal, input[dim].rows(), input[dim].columns(), input[dim].elements());
                    input[dim] *= (1.0 / (maxVal - minVal));
                }
            }
        }

        virtual void Summary() override {
            // nothing
        }

    };
} // namespace Preprocessing