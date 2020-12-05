#pragma once
#include "MatrixDS.h"
#include <array>
#include <ctime>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>

static bool TimeInitialized = false;

template<size_t M, size_t N>
MatrixDS<M, 1, double> OutputToClass(const MatrixDS<M, N, double>& in);

template<size_t N, size_t M>
auto ClassToOutput(const MatrixDS<M, 1, double>& in);

double& sigmoid(const double& in);

double& sigmoidDx(const double& in);

template<size_t N, size_t M>
MatrixDS<N, M, double>& Activate(const MatrixDS<N, M, double>& in);

template<size_t N, size_t M>
MatrixDS<N, M, double>& ActivateDerivative(const MatrixDS<N, M, double>& in);

template<size_t N, size_t M>
void InitializeWeights(MatrixDS<N, M, double>& weights, const double& minWeight, const double& maxWeight);

template<size_t N, size_t InputCount, size_t OutputCount>
std::pair<MatrixDS<N, OutputCount, double>, MatrixDS<N, OutputCount, double>> feedForward(
const MatrixDS<N, InputCount, double>& inputs,
const MatrixDS<InputCount + 1, OutputCount, double>& weights,
const MatrixDS<N, 1, double>& biases);

void InitTime() {
    std::srand((int)std::time(0));
    TimeInitialized = true;
}

template<size_t N, size_t M>
size_t Corr(const MatrixDS<N, M, double>& A, const MatrixDS<N, M, double>& B);

template<size_t InputCount, size_t OutputCount>
class Classifier
{
    template<size_t N>
    struct DataSet {
        MatrixDS<N, InputCount, double> Inputs;
        MatrixDS<N, OutputCount, double> Outputs;
        // classes
        MatrixDS<N, 1, double> Classes;
        size_t count                  = 0;
        MatrixDS<N, 1, double> Biases = MatrixDS<N, 1, double>(1.0);

        explicit DataSet(const char* fileName) { ReadFromFile(fileName); }

        void ReadFromFile(const char* fileName) {
            std::string line;
            std::ifstream dataFile(fileName);
            if(dataFile.is_open()) {
                while(getline(dataFile, line)) {
                    std::string finishedPart;
                    std::string val;
                    std::getline(dataFile, val, '\t');
                    Inputs[count][0] = std::atof(val.c_str());
                    finishedPart += val + "\t";
                    std::getline(dataFile, val, '\t');
                    Inputs[count][1] = std::atof(val.c_str());
                    finishedPart += val + "\t";
                    std::getline(dataFile, val, '\t');
                    Inputs[count][2] = std::atof(val.c_str());
                    finishedPart += val + "\t";
                    std::getline(dataFile, val, '\t');
                    Inputs[count][3] = std::atof(val.c_str());
                    finishedPart += val + "\t";
                    std::getline(dataFile, val, '\t');
                    Outputs[count][0] = std::atoi(val.c_str());
                    finishedPart += val + "\t";
                    std::getline(dataFile, val, '\t');
                    Outputs[count][1] = std::atoi(val.c_str());
                    finishedPart += val + "\t";
                    val               = line;
                    Outputs[count][2] = std::atoi(val.c_str());

                    ++count;
                }
                dataFile.close();
            } else {
                std::cout << "Unable to open file";
            }
        }

    public:
        std::pair<double, double> EvaluateNetworkError(const MatrixDS<InputCount + 1, OutputCount, double>& weights) {
            auto outputNet = feedForward(Inputs, weights, Biases);
            auto output    = outputNet.first;
            auto error     = Outputs - output;
            auto e         = HadamardMulti(error, error).sumElements() / (N * OutputCount);
            auto classes   = OutputToClass(output);
            auto c         = Corr(classes, Classes) / N;

            return std::make_pair(c, e);
        }
    };

public:
    Classifier() {
        trainingSet   = new DataSet<75>("../../resources/iris_data_files/iris_training.dat");
        validationSet = new DataSet<37>("../../resources/iris_data_files/iris_validation.dat");
        testSet       = new DataSet<38>("../../resources/iris_data_files/iris_test.dat");
    }

    template<size_t N, typename T>
    MatrixDS<InputCount + 1, OutputCount, T> BackPropagate(
    [[maybe_unused]] const DataSet<N>& ds,
    const MatrixDS<InputCount + 1, OutputCount, T>& weight,
    [[maybe_unused]] const double eta) {
        auto sampleCount = N;
        auto cols        = InputCount;

        size_t sampleIndex = ((rand() % 100) / 100.0) * sampleCount;
        MatrixDS<1, InputCount> randomInput;
        size_t i, j;
        for(i = 0; i < cols; i++) {
            for(j = 0; j < 1; j++) { randomInput[j][i] = ds.Inputs[sampleIndex + j][i]; }
        }
        MatrixDS<1, 1> bias;
        for(i = 0; i < cols; i++) {
            for(j = 0; j < 1; j++) { bias[j][i] = ds.Biases[sampleIndex + j][0]; }
        }
        MatrixDS<1, OutputCount> localErr;
        for(i = 0; i < cols; i++) {
            for(j = 0; j < 1; j++) { localErr[j][i] = ds.Outputs[sampleIndex + j][i]; }
        }
        auto q            = feedForward(randomInput, weight, bias);
        auto error        = localErr - q.first;
        auto delta        = HadamardMulti(error, ActivateDerivative(q.second));
        auto out          = HorizontalConcat(randomInput, bias).Transpose();
        auto weightsDelta = KroneckerMulti(out, delta) * eta;
        auto weights      = weight + weightsDelta;
        return weights;
    }

    void Train(const size_t maxIterations = 500, const double maxWeight = 0.5, const double eta = 0.1) {
        if(trainingSet == nullptr || validationSet == nullptr || testSet == nullptr) { return; }
        trainingErrors.clear();
        // TODO:
        //        const size_t maxEpoch = 5;
        InitializeWeights(modelWeights, 0.0, maxWeight);
        trainingErrors.resize(maxIterations + 1);
        size_t iter = 0;
        while(iter < maxIterations) {
            modelWeights = BackPropagate(*trainingSet, modelWeights, eta);
            trainingErrors[iter].resize(2);
            auto err = trainingSet->EvaluateNetworkError(modelWeights);
            trainingErrors[iter][0].push_back(err.first);
            trainingErrors[iter][1].push_back(err.second);
            err = validationSet->EvaluateNetworkError(modelWeights);
            trainingErrors[iter][0].push_back(err.first);
            trainingErrors[iter][1].push_back(err.second);
            err = testSet->EvaluateNetworkError(modelWeights);
            trainingErrors[iter][0].push_back(err.first);
            trainingErrors[iter][1].push_back(err.second);
            iter++;
            usleep(rand() % 1000);
        }
        trainingErrors[iter].resize(2);
        auto err = trainingSet->EvaluateNetworkError(modelWeights);
        trainingErrors[iter][0].push_back(err.first);
        trainingErrors[iter][1].push_back(err.second);
        err = validationSet->EvaluateNetworkError(modelWeights);
        trainingErrors[iter][0].push_back(err.first);
        trainingErrors[iter][1].push_back(err.second);
        err = testSet->EvaluateNetworkError(modelWeights);
        trainingErrors[iter][0].push_back(err.first);
        trainingErrors[iter][1].push_back(err.second);

        std::vector<double> x1;
        std::vector<double> y1;
        std::vector<double> x2;
        std::vector<double> y2;
        std::vector<double> x3;
        std::vector<double> y3;
        for(size_t i = 0; i <= maxIterations; i++) {
            x1.push_back(trainingErrors[i][0][0]);
            y1.push_back(trainingErrors[i][1][0]);
            x2.push_back(trainingErrors[i][0][1]);
            y2.push_back(trainingErrors[i][1][1]);
            x3.push_back(trainingErrors[i][0][2]);
            y3.push_back(trainingErrors[i][1][2]);
        }

        auto printVec = [](const std::vector<double>& vec) {
            std::cout << "[ ";
            for(const auto& elem : vec) { std::cout << elem << ", "; }
            std::cout << "]\n";
        };

        printVec(x1);
        printVec(y1);
        printVec(x2);
        printVec(y2);
        printVec(x3);
        printVec(y3);
    }

private:
    DataSet<75>* trainingSet   = nullptr;
    DataSet<37>* validationSet = nullptr;
    DataSet<38>* testSet       = nullptr;

    std::vector<std::vector<std::vector<double>>> trainingErrors;
    MatrixDS<InputCount + 1, OutputCount, double> modelWeights = MatrixDS<InputCount + 1, OutputCount, double>();
};


template<size_t M, size_t N>
MatrixDS<M, 1, double> OutputToClass(const MatrixDS<M, N, double>& in) {
    auto result = new MatrixDS<M, 1, double>(0);
    for(size_t i = 0; i < in.rows(); i++) {
        for(size_t j = 0; j < in.columns(); j++) {
            if(in[i][j] == 1) (*result)[i][0] = j;
        }
    }
    return *result;
}

template<size_t N, size_t M>
auto ClassToOutput(const MatrixDS<M, 1, double>& in) {
    auto result = new MatrixDS<M, N, double>(0);
    for(size_t i = 0; i < M; i++) { (*result)[i][(size_t)in[i][0]] = 1; }
    return *result;
}


double& sigmoid(const double& in) {
    auto out = new double();
    (*out)   = (tanh(in) + 1) / 2.0;
    return *out;
}
double& sigmoidDx(const double& in) {
    auto out = new double;
    (*out)   = (1 - pow(tanh(in), 2)) / 2.0;
    return *out;
}
template<size_t N, size_t M>
MatrixDS<N, M, double>& Activate(const MatrixDS<N, M, double>& in) {
    auto result = new MatrixDS<N, M, double>();
    for(size_t i = 0; i < N; ++i) {
        for(size_t j = 0; j < M; ++j) { (*result)[i][j] = sigmoid(in[i][j]); }
    }
    return *result;
}
template<size_t N, size_t M>
MatrixDS<N, M>& ActivateDerivative(const MatrixDS<N, M, double>& in) {
    auto result = new MatrixDS<N, M, double>();
    for(size_t i = 0; i < N; ++i) {
        for(size_t j = 0; j < M; ++j) { (*result)[i][j] = sigmoidDx(in[i][j]); }
    }
    return *result;
}

template<size_t N, size_t M>
void InitializeWeights(MatrixDS<N, M, double>& weights, const double& minWeight, const double& maxWeight) {
    if(!TimeInitialized) InitTime();

    const double diff = maxWeight - minWeight;

    for(size_t i = 0; i < N; ++i) {
        for(size_t j = 0; j < M; ++j) { weights[i][j] = (double)minWeight + ((rand() % 100) * diff / 100.0); }
    }
}

template<size_t N, size_t M>
size_t Corr(const MatrixDS<N, M, double>& A, const MatrixDS<N, M, double>& B) {
    size_t count = 0;
    for(size_t i = 0; i < N; i++) {
        for(size_t j = 0; j < M; j++) { count += A[i][j] == B[i][j]; }
    }
    return count;
}

template<size_t N, size_t InputCount, size_t OutputCount>
std::pair<MatrixDS<N, OutputCount, double>, MatrixDS<N, OutputCount, double>> feedForward(
const MatrixDS<N, InputCount, double>& inputs,
const MatrixDS<InputCount + 1, OutputCount, double>& weights,
const MatrixDS<N, 1, double>& biases) {
    auto net    = HorizontalConcat(inputs, biases) * weights;
    auto output = Activate(net);
    return std::make_pair(output, net);
}
