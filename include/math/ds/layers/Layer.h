#pragma once

#include "../../Matrix.h"
#include "../../Tensor.h"

template<typename T = double>
class Layer
{
public:
    std::string name{};
    std::string Type{};

    void SetName(const std::string& Name){ name = Name; }
    /**
     * Replace defines a virtual interface to process a tensor.
     * Implement this method in your layer.
     * @param input
     */
    virtual void Replace(Tensor<T>& input) const = 0;

    /**
     * Copies a tensor and applies `Replace` onto it
     * @param input tensor to process
     * @return processed tensor
     */
    [[nodiscard]] Tensor<T> Process(const Tensor<T>& input) const {
        // create real copy of tensor
        Tensor<T> out = input;
        Replace(out);
        return out;
    }

    virtual void Summary() = 0;
};