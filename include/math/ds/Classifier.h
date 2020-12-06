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
MatrixDS<M, 1, double> &OutputToClass(const MatrixDS<M, N, double>& in);

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

void InitTime(bool useSeed = true) {
    std::srand(useSeed ? (int)std::time(0) : 2);
    TimeInitialized = true;
}

template<size_t InputCount, size_t OutputCount, size_t TrainingSetSize, size_t ValidationSetSize, size_t TestSetSize>
class Classifier
{
    struct EvaluationError {
        double Regression;
        double Classification;
    };
    struct EvaluationErrorSet {
        EvaluationError Training;
        EvaluationError Validation;
        EvaluationError Test;
    };
    struct EvaluationStatistics {
        double RegressionMean = 0.0;
        double RegressionStd  = 0.0;
        double RegressionMax  = std::numeric_limits<double>::min();

        double ClassificationMean = 0.0;
        double ClassificationStd  = 0.0;
        double ClassificationMax  = std::numeric_limits<double>::min();

        double RegressionSum            = 0.0;
        double RegressionSumSquared     = 0.0;
        double ClassificationSum        = 0.0;
        double ClassificationSumSquared = 0.0;

        size_t ElementCount = 0;

        void Calc() {
            RegressionMean     = RegressionSum / ElementCount;
            ClassificationMean = ClassificationSum / ElementCount;

            RegressionStd =
            (RegressionSumSquared - (RegressionSum * RegressionSum) / ElementCount) / (ElementCount - 1);
            ClassificationStd =
            (ClassificationSumSquared - (ClassificationSum * ClassificationSum) / ElementCount) / (ElementCount - 1);
        }

        friend std::ostream& operator<<(std::ostream& ostr, const EvaluationStatistics& err) {
            ostr << "Regression:\n\tMean: " << err.RegressionMean << " Std: " << err.RegressionStd
                 << " Max: " << err.RegressionMax << "\n";
            ostr << "Classification:\n\tMean: " << err.ClassificationMean << " Std: " << err.ClassificationStd
                 << " Max: " << err.ClassificationMax << "\n";
            return ostr;
        }
    };
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

                Classes = OutputToClass(Outputs);
            } else {
                std::cout << "Unable to open file";
            }
        }

        EvaluationError EvaluateNetworkError(const MatrixDS<InputCount + 1, OutputCount, double>& weights) {
            auto outputNet = feedForward(Inputs, weights, Biases);
            auto output    = outputNet.first;
            auto net       = outputNet.second;
            auto error     = output - Outputs;
            auto errorSqr  = HadamardMulti(error, error);
            // sum(sum((outputs - data_set.outputs) .^2)) / (data_set.count * output_count);
            auto regression = errorSqr.sumElements() / (double)(N * OutputCount);
            auto classes   = OutputToClass(output);
            auto classification = (double)Corr(classes, Classes) /(double) N;

            return EvaluationError(regression, classification);
        }
    };

public:
    Classifier() {
        trainingSet   = new DataSet<TrainingSetSize>("../../resources/iris_data_files/iris_training.dat");
        validationSet = new DataSet<ValidationSetSize>("../../resources/iris_data_files/iris_validation.dat");
        testSet       = new DataSet<TestSetSize>("../../resources/iris_data_files/iris_test.dat");
    }

    template<size_t N, typename T>
    MatrixDS<InputCount + 1, OutputCount, T> BackPropagate(
    [[maybe_unused]] const DataSet<N>& ds,
    const MatrixDS<InputCount + 1, OutputCount, T>& weight,
    [[maybe_unused]] const double eta) {
        auto sampleCount = N;
        auto cols        = InputCount;

        size_t sampleIndex = ((rand() % 100) / 100.0) * sampleCount;
        std::cout << "sampleIndex: " << sampleIndex << std::endl;
        MatrixDS<1, InputCount> randomInput;
        size_t i;
        for(i = 0; i < cols; i++) {
            randomInput[0][i] = ds.Inputs[sampleIndex][i];
        }
        MatrixDS<1, 1> bias;

        bias[0][0] = ds.Biases[sampleIndex][0];
        MatrixDS<1, OutputCount> localErr;
        for(i = 0; i < OutputCount; i++) {
            localErr[0][i] = ds.Outputs[sampleIndex][i];
        }
        auto q            = feedForward(randomInput, weight, bias);
        auto error        = localErr - q.first;
        auto delta        = HadamardMulti(error, ActivateDerivative(q.second));
        auto out          = HorizontalConcat(bias, randomInput).Transpose();
        auto weightsDelta = KroneckerMulti(delta, out) * eta;
        auto weights      = weight + weightsDelta;
        return weights;
    }

    void Train(const size_t maxIterations = 5000, const double maxWeight = 0.5, const double eta = 0.01) {
        if(trainingSet == nullptr || validationSet == nullptr || testSet == nullptr) { return; }
        trainingErrors.clear();
        InitializeWeights(modelWeights, -maxWeight, maxWeight);
        trainingErrors.resize(maxIterations + 1);
        size_t iter          = 0;
        double stopThreshold = 0.1;

        std::cout << "Training:\n";
        while(iter < maxIterations) {
            std::cout << "Epoch: " << iter << '\n';
            modelWeights = BackPropagate(*trainingSet, modelWeights, eta);
            EvaluationErrorSet errorSet;
            errorSet.Training = trainingSet->EvaluateNetworkError(modelWeights);
            errorSet.Validation = validationSet->EvaluateNetworkError(modelWeights);
            errorSet.Test = testSet->EvaluateNetworkError(modelWeights);
            trainingErrors[iter] = errorSet;

            if(errorSet.Validation.Regression < stopThreshold) { break; }

            if(iter % 30 == 0 && iter != 0) {
                EvaluationStatistics stats;
                stats.ElementCount = 30;
                for(size_t i = iter - 30; i < iter; i++) {
                    stats.RegressionSum += trainingErrors[i].Validation.Regression;
                    stats.ClassificationSum += trainingErrors[i].Validation.Classification;
                    stats.RegressionSumSquared += (trainingErrors[i].Validation.Regression * trainingErrors[i].Validation.Regression);
                    stats.ClassificationSumSquared += (trainingErrors[i].Validation.Classification * trainingErrors[i].Validation.Classification);
                    if(trainingErrors[i].Validation.Regression > stats.RegressionMax) { stats.RegressionMax = trainingErrors[i].Validation.Regression; }
                    if(trainingErrors[i].Validation.Classification > stats.ClassificationMax) {
                        trainingErrors[i].Validation.Classification;
                    }
                }
                stats.Calc();

                std::cout << stats;
            }

            iter++;
        }
        auto trainingError   = trainingSet->EvaluateNetworkError(modelWeights);
        auto validationError = validationSet->EvaluateNetworkError(modelWeights);
        auto testError       = testSet->EvaluateNetworkError(modelWeights);

        std::cout << "Training: " << trainingError.Regression << ", " << trainingError.Classification << '\n';
        std::cout << "Validation: " << validationError.Regression << ", " << validationError.Classification << '\n';
        std::cout << "Test: " << testError.Regression << ", " << testError.Classification << '\n';

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
            dataFile << "\n"
                        "figure()\n"
                        "hold on\n"
                        "plot(y,x1)\n"
                        "plot(y,x2)\n"
                        "plot(y,x3)\n"
                        "plot(y,x4)\n"
                        "plot(y,x5)\n"
                        "plot(y,x6)\n"
                        "ylim([0,1])\n"
                        "legend(\"Regression Training\", \"Classification Training\", \"Regression Validation\", "
                        "\"Classification Validation\", \"Regression Test\", \"Validation Test\");";
        } else {
            std::cout << "Could not open file.\n";
        }
        {
            EvaluationStatistics stats;
            stats.ElementCount = iter;
            for(size_t i = 0; i < iter; i++) {
                stats.RegressionSum += trainingErrors[i].Validation.Regression;
                stats.ClassificationSum += trainingErrors[i].Validation.Classification;
                stats.RegressionSumSquared += (trainingErrors[i].Validation.Regression * trainingErrors[i].Validation.Regression);
                stats.ClassificationSumSquared += (trainingErrors[i].Validation.Classification * trainingErrors[i].Validation.Classification);
                if(trainingErrors[i].Validation.Regression > stats.RegressionMax) { stats.RegressionMax = trainingErrors[i].Validation.Regression; }
                if(trainingErrors[i].Validation.Classification > stats.ClassificationMax) {
                    trainingErrors[i].Validation.Classification;
                }
            }
            stats.Calc();

            std::cout << stats;
        }
        std::cout << "Weights:\n" << modelWeights;
    }

private:
    DataSet<TrainingSetSize>* trainingSet   = nullptr;
    DataSet<ValidationSetSize>* validationSet = nullptr;
    DataSet<TestSetSize>* testSet       = nullptr;

    std::vector<EvaluationErrorSet> trainingErrors;
    MatrixDS<InputCount + 1, OutputCount, double> modelWeights = MatrixDS<InputCount + 1, OutputCount, double>();
};


template<size_t M, size_t N>
MatrixDS<M, 1, double>& OutputToClass(const MatrixDS<M, N, double>& in) {
    auto result = new MatrixDS<M, 1, double>(0);
    for(size_t i = 0; i < in.rows(); i++) {
        int rowMax = -1;
        double rowMaxVal = -1;
        for(size_t j = 0; j < in.columns(); j++) {
            if(in[i][j] > rowMaxVal) { rowMaxVal = in[i][j]; rowMax = j; }
        }
        if(rowMax != -1){
            (*result)[i][0] = rowMax;
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
//    if(!TimeInitialized) InitTime(false);

    const double diff = maxWeight - minWeight;

    for(size_t i = 0; i < N; ++i) {
        for(size_t j = 0; j < M; ++j) { weights[i][j] = (double)minWeight + ((rand() % 100) * diff / 100.0); }
    }
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
