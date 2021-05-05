#pragma once

#include "Layer.h"


template<typename T>
class PoolingLayer : public Layer<T>
{
    using TTensor = Tensor<T>; // typed tensor

    enum PoolingMode {
        MAX = 0,
        AVG = 1,
    };
    size_t stride{}; // step width
    size_t f{};      // filter size
    PoolingMode mode;

public:
    explicit PoolingLayer(size_t filterSize = 2, size_t stepStride = 0, PoolingMode poolingMode = PoolingMode::MAX)
        : Layer<T>()
        , stride(stepStride)
        , f(filterSize)
        , mode(poolingMode) {this->Type = "pooling";}

    /**
     * Note: Doesn't change the size of input
     * @param input
     */
    virtual void Replace(Tensor<T>& input) const {
        auto m = input.dim;
        if(m <= 0) {
            // error
        }
        auto n_H_prev = input[0].rows();
        auto n_W_prev = input[0].columns();
        auto n_C_prev = input[0].elements();

        auto n_H = int(1 + (n_H_prev - f) / stride);
        auto n_W = int(1 + (n_W_prev - f) / stride);

        auto n_C = n_C_prev;

        auto out = TTensor(m, n_C);
        for(size_t i = 0; i < m; ++i) {
            out.push_back(Matrix<T>(0, n_H, n_W, n_C));
            auto img = input[i];
            for(size_t w = 0; w < n_W; ++w) {
                for(size_t h = 0; h < n_H; ++h) {
                    auto w_range = stride * w;
                    auto h_range = stride * h;

                    auto img_slice = img.GetSlice(h_range, h_range + f - 1, w_range, w_range + f - 1);
                    switch(mode) {
                        case PoolingMode::AVG:
                            {
                                for(size_t c = 0; c < n_C; ++c) { out[i](h, w, c) = elemMean(img_slice, c); }
                            }
                            break;
                        default:
                        case PoolingMode::MAX:
                            {
                                for(size_t c = 0; c < n_C; ++c) { out[i](h, w, c) = elemMax(img_slice, c); }
                            }
                            break;
                    }
                }
            }
        }
        input = out;
    }

    virtual void Summary() override {
        std::cout << this->name << "\t\t\t\t\t(None, " << f << ", " << f << ", any)\t\t\t\t\t" << "" /* */ << "\n"
         "_________________________________________________________________\n";
    }

};