#pragma once
#include "../../Tensor.h"
#include "Layer.h"

class PaddingLayer : public Layer<double>
{
    double elementValue;
    size_t numElements = 0;

public:
    PaddingLayer(const double& val, const size_t& elements = 2)
        : Layer<double>()
        , elementValue(val)
        , numElements(elements) {this->Type = "padding";}

    virtual void Replace(Tensor<double>& input) const {
        // Essentially we transpose a matrix to add a row
        for(auto& elem : input) {
            // right
            auto out = elem.HorizontalConcat(Matrix<>(elementValue, elem.rows(), numElements, elem.elements()));
            // left
            out = HorizontalConcat(Matrix<>(elementValue, elem.rows(), numElements, elem.elements()), out).Transpose();
            //top
            out = out.HorizontalConcat(Matrix<>(elementValue, out.rows(), numElements, elem.elements()));
            // bottom
            elem = HorizontalConcat(Matrix<>(elementValue, out.rows(), numElements, elem.elements()), out).Transpose();
        }
    }

    virtual void Summary() override {
        // Nothing
    }

};
