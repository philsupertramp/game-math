#pragma once
#include <stdlib.h>
#include "MatrixDS.h"
#include "any.h"
#include <ranges>

double sigmoid(const double &in){return 1.0/(1.0+exp(in));}
double sigmoidDX(const double &in){return in * (1.0-in);}

template<size_t N, size_t M>
MatrixDS<N, M, double> Activate(const MatrixDS<N, M, double> &in){
    auto out = new MatrixDS<N, M, double>();
    for(size_t i = 0; i < N; i++){
        for(size_t j = 0; j < M; j++){
            (*out)[i][j] = sigmoid(in[i][j]);
        }
    }
    return *out;
}

template<size_t In, size_t Out>
class NNLayer
{
public:
    NNLayer(){ }

    MatrixDS<Out, 1, double> Evaluate(const MatrixDS<In, 1, double>& input){
        return Activate((weights * input) + bias);
    }

    MatrixDS<In, Out, double> weights = MatrixDS<In, Out, double>::Random();
    MatrixDS<In, 1, double> bias = MatrixDS<In, 1, double>::Random();

    constexpr size_t Prev() { return in; }
    constexpr size_t Next() { return out; }

    const size_t in = In;
    const size_t out = Out;
};

template<size_t In, size_t Out>
class NN
{

public:
    NN(){
        numLayers = 1;
        layers.resize(1);
        layers[0] = NNLayer<In, Out>();
    }

    std::tuple<MatrixDS<Out,1,double>,MatrixDS<Out,1,double>> FeedForward(){
        MatrixDS<In, 1, double> in(0);
        auto out = any_cast<NNLayer<In,Out>>(layers[0]).Evaluate(in);
        MatrixDS<Out, 1, double> expectedOut(0);
        auto err = expectedOut - out;
        return std::make_tuple(out, err);
    }

    /**
     * Keep in mind, prev and next need to match in and output!
     * @tparam Prev
     * @tparam Next
     */
    template<size_t Prev, size_t Next>
    void AddLayer(){
        auto curSize = layers.size();
        layers.resize(curSize + 1);
        if(curSize >= 1){
            auto prevLayer = any_cast<NNLayer<In,Out>>(layers[curSize-1]);
            layers[curSize-1] = NNLayer<In, Prev>();
        }
        layers[curSize] = NNLayer<Prev, Next>();
        numLayers++;
    }

    size_t numLayers;
    std::vector<any> layers;
};
