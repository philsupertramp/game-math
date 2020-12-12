#pragma once
#include "MatrixDS.h"
#include "Evaluation.h"
#include <cstring>
#include <fstream>

template<size_t InputCount, size_t OutputCount, size_t N>
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
        auto output         = outputNet.Output;
        auto error          = output - Outputs;
        auto errorSqr       = HadamardMulti(error, error);
        auto regression     = errorSqr.sumElements() / (double)(N * OutputCount);
        auto classes        = OutputToClass(output);
        auto classification = (double)Corr(classes, Classes) / (double)N;

        return EvaluationError(regression, classification);
    }
};
template<size_t InputCount, size_t OutputCount, size_t TrainingSetSize, size_t ValidationSetSize, size_t TestSetSize>
struct DataSetSet {
    DataSet<InputCount, OutputCount, TrainingSetSize>* trainingSet     = nullptr;
    DataSet<InputCount, OutputCount, ValidationSetSize>* validationSet = nullptr;
    DataSet<InputCount, OutputCount, TestSetSize>* testSet             = nullptr;

    explicit DataSetSet(const char* filePath){
        char trainingsFile[55];
        strcpy(trainingsFile, filePath); strcat(trainingsFile, "training.dat");
        char validationFile[55];
        strcpy(validationFile, filePath);  strcat(validationFile, "validation.dat");
        char testFile[55];
        strcpy(testFile, filePath);  strcat(testFile, "test.dat");
        trainingSet   = new DataSet<InputCount, OutputCount, TrainingSetSize>(trainingsFile);
        validationSet = new DataSet<InputCount, OutputCount, ValidationSetSize>(validationFile);
        testSet       = new DataSet<InputCount, OutputCount, TestSetSize>(testFile);
    }
};

template<size_t InputCount, size_t OutputCount>
class BaseNNModel
{
public:
    template<size_t TrainingSetSize, size_t ValidationSetSize, size_t TestSetSize>
    void Train(const char*, size_t, double, double, double, bool, bool){ };


    virtual MatrixDS<1, 1> Predict(MatrixDS<1, InputCount, double> input) = 0;
};