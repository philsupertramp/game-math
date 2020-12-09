#pragma once
#include "../Random.h"
#include "../format.h"
#include "Evaluation.h"
#include "MatrixDS.h"
#include <array>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <unistd.h>
#include <vector>

#ifndef EPS
#define EPS 1e-12
#endif


template<size_t M, size_t N>
MatrixDS<M, 1, double>& OutputToClass(const MatrixDS<M, N, double>& in);

template<size_t N, size_t M>
auto ClassToOutput(const MatrixDS<M, 1, double>& in);

double sigmoid(const double& in);

double sigmoidDx(const double& in);

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

/**
 * TODO:
 * @tparam InputCount
 * @tparam OutputCount
 * @tparam TrainingSetSize
 * @tparam ValidationSetSize
 * @tparam TestSetSize
 */
template<size_t InputCount, size_t OutputCount>
class Classifier
{
    /**
     *
     * @tparam N
     */
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
                    std::string val;
                    for(size_t i = 0; i < InputCount + OutputCount; i++) {
                        if(i < InputCount + OutputCount - 1) { std::getline(dataFile, val, '\t'); }
                        else { val = line; }
                        if(i < InputCount){
                            Inputs[count][i]  = std::atof(val.c_str());
                        } else {
                            Outputs[count][i - InputCount] = std::atoi(val.c_str());
                        }
                    }
                    ++count;
                }
                dataFile.close();
                if(count != N) {
                    std::cerr << "Found less data then expected!\n";
                }
                Classes = OutputToClass(Outputs);
            } else {
                std::cerr << "Unable to open file";
            }
        }

        EvaluationError EvaluateNetworkError(const MatrixDS<InputCount + 1, OutputCount, double>& weights) {
            auto outputNet      = feedForward(Inputs, weights, Biases);
            auto output         = outputNet.first;
            auto net            = outputNet.second;
            auto error          = output - Outputs;
            auto errorSqr       = HadamardMulti(error, error);
            auto regression     = errorSqr.sumElements() / (double)(N * OutputCount);
            auto classes        = OutputToClass(output);
            auto classification = (double)Corr(classes, Classes) / (double)N;

            return EvaluationError(regression, classification);
        }
    };
    template<size_t TrainingSetSize, size_t ValidationSetSize, size_t TestSetSize>
    struct DataSetSet {
        DataSet<TrainingSetSize>* trainingSet     = nullptr;
        DataSet<ValidationSetSize>* validationSet = nullptr;
        DataSet<TestSetSize>* testSet             = nullptr;

        explicit DataSetSet(const char* filePath){
            char trainingsFile[55];
            strcpy(trainingsFile, filePath); strcat(trainingsFile, "training.dat");
            char validationFile[55];
            strcpy(validationFile, filePath);  strcat(validationFile, "validation.dat");
            char testFile[55];
            strcpy(testFile, filePath);  strcat(testFile, "test.dat");
            trainingSet   = new DataSet<TrainingSetSize>(trainingsFile);
            validationSet = new DataSet<ValidationSetSize>(validationFile);
            testSet       = new DataSet<TestSetSize>(testFile);
        }
    };

public:
    Classifier() = default;

    template<size_t N, typename T>
    MatrixDS<InputCount + 1, OutputCount, T> BackPropagate(
    [[maybe_unused]] const DataSet<N>& ds,
    const MatrixDS<InputCount + 1, OutputCount, T>& weight,
    [[maybe_unused]] const double eta) {
        /** generate necessary  */
        auto sampleCount = N;
        auto cols        = InputCount;
        size_t sampleIndex = Random::Get(0, (double)sampleCount-1.0);
        MatrixDS<1, InputCount> randomInput;
        size_t i;
        for(i = 0; i < cols; i++) { randomInput[0][i] = ds.Inputs[sampleIndex][i]; }
        MatrixDS<1, 1> bias;
        bias[0][0] = ds.Biases[sampleIndex][0];
        MatrixDS<1, OutputCount> localErr;
        for(i = 0; i < OutputCount; i++) { localErr[0][i] = ds.Outputs[sampleIndex][i]; }

        /** actual back propagation */
        auto q            = feedForward(randomInput, weight, bias);
        auto error        = localErr - q.first;
        auto delta        = HadamardMulti(error, q.second);
        auto out          = HorizontalConcat(bias, randomInput).Transpose();
        auto weightsDelta = KroneckerMulti(delta, out) * eta;
        auto weights      = weight + weightsDelta;
        return weights;
    }

    template<size_t TrainingSetSize, size_t ValidationSetSize, size_t TestSetSize>
    void Train(const char* filePath, const size_t maxIterations = 5000, const double maxWeight = 0.5, const double eta = 0.01, const double stopThreshold = 0.1, bool reuseWeights = false) {
        DataSetSet<TrainingSetSize, ValidationSetSize, TestSetSize> DS(filePath);
        trainingErrors.clear();
        if(!reuseWeights) InitializeWeights(modelWeights, -maxWeight, maxWeight);

        trainingErrors.resize(maxIterations + 1);
        size_t iter          = 0;

        EvaluationStatistics currentStats;
        while(iter < maxIterations) {
            modelWeights = BackPropagate(*DS.trainingSet, modelWeights, eta);
            EvaluationErrorSet errorSet = EvaluationErrorSet();
            errorSet.Training    = DS.trainingSet->EvaluateNetworkError(modelWeights);
            errorSet.Validation  = DS.validationSet->EvaluateNetworkError(modelWeights);
            errorSet.Test        = DS.testSet->EvaluateNetworkError(modelWeights);
            trainingErrors[iter] = errorSet;

            if(errorSet.Validation.Regression < stopThreshold) {
                std::cout << "Met threshold!\n";
                break;
            }

            if(iter % 30 == 0 && iter != 0) {
                currentStats = EvaluationStatistics();
                currentStats.ElementCount = 30;
                for(size_t i = iter - 30; i < iter; i++) {
                    currentStats.RegressionSum += trainingErrors[i].Validation.Regression;
                    currentStats.ClassificationSum += trainingErrors[i].Validation.Classification;
                    currentStats.RegressionSumSquared +=
                    (trainingErrors[i].Validation.Regression * trainingErrors[i].Validation.Regression);
                    currentStats.ClassificationSumSquared +=
                    (trainingErrors[i].Validation.Classification * trainingErrors[i].Validation.Classification);
                    if(trainingErrors[i].Validation.Regression > currentStats.RegressionMax) {
                        currentStats.RegressionMax = trainingErrors[i].Validation.Regression;
                    }
                    if(trainingErrors[i].Validation.Classification > currentStats.ClassificationMax) {
                        currentStats.ClassificationMax = trainingErrors[i].Validation.Classification;
                    }
                }
                currentStats.Calc();
            }

            iter++;
        }
        finalErrors.Training   = DS.trainingSet->EvaluateNetworkError(modelWeights);
        finalErrors.Validation = DS.validationSet->EvaluateNetworkError(modelWeights);
        finalErrors.Test       = DS.testSet->EvaluateNetworkError(modelWeights);

        std::cout << "Training: " << finalErrors.Training.Regression << ", " << finalErrors.Training.Classification
                  << '\n';
        std::cout << "Validation: " << finalErrors.Validation.Regression << ", "
                  << finalErrors.Validation.Classification << '\n';
        std::cout << "Test: " << finalErrors.Test.Regression << ", " << finalErrors.Test.Classification << '\n';

        std::vector<double> x1;
        std::vector<double> y1;
        std::vector<double> x2;
        std::vector<double> y2;
        std::vector<double> x3;
        std::vector<double> y3;
        std::vector<double> y;
        for(size_t i = 0; i < maxIterations; i++) {
            x1.push_back(trainingErrors[i].Training.Regression);
            y1.push_back(trainingErrors[i].Training.Classification);
            x2.push_back(trainingErrors[i].Validation.Regression);
            y2.push_back(trainingErrors[i].Validation.Classification);
            x3.push_back(trainingErrors[i].Test.Regression);
            y3.push_back(trainingErrors[i].Test.Classification);
            y.push_back(i + 1);
        }

        auto printVec = [](const std::vector<double>& vec, std::ostream& ostr = std::cout) {
            ostr << "[ ";
            for(const auto& elem : vec) { ostr << elem << ", "; }
            ostr << "];\n";
        };


        std::string line;
        std::ofstream dataFile("../../resources/iris_data_files/script.m");
        if(dataFile.is_open()) {
            dataFile << "y=";
            printVec(y, dataFile);
            dataFile << "x1=";
            printVec(x1, dataFile);
            dataFile << "x2=";
            printVec(y1, dataFile);
            dataFile << "x3=";
            printVec(x2, dataFile);
            dataFile << "x4=";
            printVec(y2, dataFile);
            dataFile << "x5=";
            printVec(x3, dataFile);
            dataFile << "x6=";
            printVec(y3, dataFile);
            dataFile << format("\n"
                        "figure()\n"
                        "hold on\n"
                        "plot(y,x1)\n"
                        "plot(y,x2)\n"
                        "plot(y,x3)\n"
                        "plot(y,x4)\n"
                        "plot(y,x5)\n"
                        "plot(y,x6)\n"
                        "ylim([0,1])\n"
                        "xlim([0, %d])\n"
                        "legend(\"Regression Training\", \"Classification Training\", \"Regression Validation\", "
                        "\"Classification Validation\", \"Regression Test\", \"Validation Test\")\n"
                        "title(\"Error vs Epoch, conversion after %d iterations\")", iter, iter);
        } else {
            std::cerr << "Could not open file.\n";
        }
        {
            finalStats              = EvaluationStatistics();
            finalStats.ElementCount = iter;
            for(size_t i = 0; i < iter; i++) {
                finalStats.RegressionSum += trainingErrors[i].Validation.Regression;
                finalStats.ClassificationSum += trainingErrors[i].Validation.Classification;
                finalStats.RegressionSumSquared +=
                (trainingErrors[i].Validation.Regression * trainingErrors[i].Validation.Regression);
                finalStats.ClassificationSumSquared +=
                (trainingErrors[i].Validation.Classification * trainingErrors[i].Validation.Classification);
                if(trainingErrors[i].Validation.Regression > finalStats.RegressionMax) {
                    finalStats.RegressionMax = trainingErrors[i].Validation.Regression;
                }
                if(trainingErrors[i].Validation.Classification > finalStats.ClassificationMax) {
                    finalStats.ClassificationMax = trainingErrors[i].Validation.Classification;
                }
            }
            finalStats.Calc();
        }
    }

    MatrixDS<OutputCount, 1, double> Evaluate(MatrixDS<InputCount, 1, double> input){
        return input.HorizontalConcat(MatrixDS<InputCount, 1, double>(1.0)) * modelWeights;
    }

    EvaluationErrorSet finalErrors{};
    EvaluationStatistics finalStats;

private:

    std::vector<EvaluationErrorSet> trainingErrors;
    MatrixDS<InputCount + 1, OutputCount, double> modelWeights = MatrixDS<InputCount + 1, OutputCount, double>();
};


template<size_t M, size_t N>
MatrixDS<M, 1, double>& OutputToClass(const MatrixDS<M, N, double>& in) {
    auto result = new MatrixDS<M, 1, double>(0);
    for(size_t i = 0; i < in.rows(); i++) {
        int rowMax       = -1;
        double rowMaxVal = -1;
        for(size_t j = 0; j < in.columns(); j++) {
            if(in[i][j] > rowMaxVal) {
                rowMaxVal = in[i][j];
                rowMax    = j;
            }
        }
        if(rowMax != -1) { (*result)[i][0] = rowMax; }
    }
    return *result;
}

template<size_t N, size_t M>
auto ClassToOutput(const MatrixDS<M, 1, double>& in) {
    auto result = new MatrixDS<M, N, double>(0);
    for(size_t i = 0; i < M; i++) { (*result)[i][(size_t)in[i][0]] = 1; }
    return *result;
}


double sigmoid(const double& in) { return (tanh(in) + 1.0) / 2.0; }
double sigmoidDx(const double& in) {
    auto tanVal = tanh(in);
    return (1.0 - (tanVal * tanVal)) / 2.0;
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
    for(size_t i = 0; i < N; ++i) {
        for(size_t j = 0; j < M; ++j) { weights[i][j] = Random::Get(minWeight, maxWeight); }
    }
}


template<size_t N, size_t InputCount, size_t OutputCount>
std::pair<MatrixDS<N, OutputCount, double>, MatrixDS<N, OutputCount, double>> feedForward(
const MatrixDS<N, InputCount, double>& inputs,
const MatrixDS<InputCount + 1, OutputCount, double>& weights,
const MatrixDS<N, 1, double>& biases) {
    auto net    = HorizontalConcat(inputs, biases) * weights;
    auto output = net; //Activate(net);
    return std::make_pair(output, net);
}
