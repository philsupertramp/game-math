#pragma once

#include "../../Tensor.h"
#include "../Classifier.h"
#include "../utils.h"
#include "Layer.h"
#include "PaddingLayer.h"


template<typename T = double>
class ConvolutionalLayer : public Layer<T>
{
    using TTensor = Tensor<T>; // typed tensor

    // TODO: allow (n_C, f, f, n_C_prev)
    TTensor filters; // <@! shape (n_C, f, f, n_C)
    Matrix<T> bias;
    size_t pad{};    // padding of each image
    size_t stride{}; // moving step
    PaddingLayer paddingLayer;

public:
    ConvolutionalLayer(size_t rows, size_t dimensions, size_t padding = 2, size_t stepStride = 2, size_t elementSize = 3)
        : Layer<T>()
        , pad(padding)
        , paddingLayer(PaddingLayer(padding, elementSize)) {
        this->Type = "convolutional";
        if(rows <= 0 || dimensions <= 0) {
            // error!!!
        }
        filters = TTensor(dimensions, elementSize);
        stride = stepStride;
        for(size_t i = 0; i < dimensions; ++i) { filters.push_back(Matrix<T>::Random(rows, rows, elementSize)); }
        bias = Matrix<double>::Random(1, 1, elementSize);
    }
    ~ConvolutionalLayer() { }

    /**
     *
     * Example:
     *  input:
     *      std::vector<Matrix<vec3d>> input = {
     *          {
     *              {1, 2, 3},
     *              {1, 2, 3}
     *          }
     *      };
     *      m = 1
     *      n_H_prev = 1
     *      n_W_prev = 2
     *      n_C_prev = 3
     * @param input shape (m, n_H_prev, n_W_prev, n_C_prev) with m batch size, n_W and n_H_prev width and height of each Matrix and n_C_prev the number of elements in each element of the tensor
     */
    void Replace(TTensor& input) const {
        auto batchSize = input.dim;
        if(batchSize <= 0) {
            // error
            return;
        }
        auto n_H_prev = input[0].rows();
        auto n_W_prev = input[0].columns();
        if(n_H_prev <= 0 || n_W_prev <= 0) {
            // error
            return;
        }
        auto n_C_prev = input[0].elements();
        auto n_C      = filters[0].elements();
        if(n_C_prev != n_C) {
            // error
            return;
        }
        auto f = filters[0].rows();
        auto n_H = int((n_H_prev - f + 2 * pad) / stride) + 1;
        auto n_W = int((n_W_prev - f + 2 * pad) / stride) + 1;

        auto out         = TTensor(batchSize, n_C);
        auto paddedInput = zeroPads(input);

        for(size_t i = 0; i < input.dim; ++i) {
            out.push_back(Matrix<T>(0, n_H, n_W, n_C));
            auto padded_img = paddedInput[i];
            for(size_t h = 0; h < n_H; ++h) {
                auto h_range = stride * h;
                for(size_t w = 0; w < n_W; ++w) {
                    for(size_t c = 0; c < n_C; ++c) {
                        auto w_range   = stride * w;
                        auto img_slice = padded_img.GetSlice(h_range, h_range + f - 1, w_range, w_range + f - 1);
                        auto weights = filters.GetComponents(c);
                        out[i](h, w, c)   = oneStepConv(img_slice, weights, bias(0, 0, c));
                    }
                }
            }
        }
        input = out;
    }

    TTensor zeroPads(const TTensor& input) const { return paddingLayer.Process(input); }

    T oneStepConv(const Matrix<T>& in, const Tensor<T>& weights, const T& b) const {
        Matrix<T> out(0, in.rows(), in.columns(), 1);
        for(size_t i = 0; i < weights.dim; ++i){
            for(size_t dim = 0; dim < in.elements(); ++dim) { out += HadamardMulti(in.GetComponents(dim), weights[i]); }
        }
        auto Z = out.sumElements();
        return Z + b;
    }

    virtual void Summary() override {
        std::cout << this->name << "\t\t\t\t(" << filters.dim << ", " << filters[0].rows() << ", "
                  << filters[0].columns() << ", " << filters[0].elements() << ")\t\t\t\t\t" << "" /* */ << "\n"
            "_________________________________________________________________\n";
    }
};