#pragma once

#include "Layer.h"


template<typename U>
class FlattenLayer : public Layer<U>
{
public:
    FlattenLayer()
    : Layer<U>()
    {
        this->Type = "flatten";
    }
    /**
     * Flattens a given tensor to a 1D-tensor using a row-vector to represent the data
     * @param input ND-Tensor
     */
    virtual void Replace(Tensor<U>& input) const {
        auto m = input.dim;
        auto H = input[0].rows();
        auto W = input[0].columns();
        auto c = input[0].elements();
        Matrix<U> out(0, m * H * W * c, 1, 1);
        size_t index = 0;
        for(size_t i = 0; i < m; ++i) {
            for(size_t j = 0; j < H; ++j) {
                for(size_t k = 0; k < W; ++k) {
                    for(size_t v = 0; v < c; ++v) { out(index++, 0) = input[i](j, k, v); }
                }
            }
        }
        input = Tensor<U>(1, 1);
        input.push_back(out);
    }
    virtual void Summary() override {
        // nothing
    }

};
