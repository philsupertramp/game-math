#pragma once

#include "../format.h"
#include "MatrixDS.h"
#include <fstream>
#include <map>
#include <sstream>

enum NormalizerMethod {
    SET_MEAN = 0,
    COL_MEAN = 1,
    ROW_MEAN = 2
};
struct Set {

    MatrixDS<double> Input;
    MatrixDS<double> Output;
    size_t InputCount = 0;
    size_t OutputCount = 0;
    size_t count = 0;

    Set() = default;

    Set(size_t inputCount, size_t outputCount) {
        InputCount  = inputCount;
        OutputCount = outputCount;
    }

    Set(const char* fileName, size_t inputCount, size_t outputCount) {
        InputCount  = inputCount;
        OutputCount = outputCount;
        ReadFromFile(fileName);
    }

    void ReadFromFile(const char* fileName) {
        std::string line;
        size_t lineCount = 0;

        std::ifstream dataFile2(fileName);
        if(dataFile2.is_open()) {
            while(getline(dataFile2, line, '\n')) { lineCount++; }
            dataFile2.close();
        }
        std::ifstream dataFile(fileName);
        count = 0;
        Input        = MatrixDS<double>(0, lineCount, InputCount);
        Output       = MatrixDS<double>(0, lineCount, OutputCount);
        if(dataFile.is_open()) {
            line = "";
            while(getline(dataFile, line, '\n')) {
                std::string val;
                std::stringstream lineStream(line);
                for(size_t i = 0; i < InputCount + OutputCount; i++) {
                    if(i < InputCount + OutputCount - 1) {
                        std::getline(lineStream, val, '\t');
                    } else {
                        std::getline(lineStream, val);
                    }
                    if(i < InputCount) {
                        Input[count][i] = std::atof(val.c_str());
                    } else {
                        Output[count][i - InputCount] = std::atof(val.c_str());
                    }
                }
                ++count;
            }
            dataFile.close();
            //            Classes = OutputToClass(Output);
        } else {
            std::cerr << "DataSet::ReadFromFile: Unable to open file " << fileName << std::endl;
        }
    }

    [[nodiscard]] Set GetBatch(int batchSize) const {
        if(batchSize == -1 || batchSize == (int)count){
            return *this;
        }

        auto newDS = Set(InputCount, OutputCount);
        newDS.Input = MatrixDS<double>(0, batchSize, InputCount);
        newDS.Output = MatrixDS<double>(0, batchSize, OutputCount);
        for(int i = 0; i < batchSize; i++){
            int index = (int)(Random::Get() * count);
            for(size_t in = 0; in < InputCount; in++){
                newDS.Input[i][in] = Input[index][in];
            }
            for(size_t out = 0; out < OutputCount; out++){
                newDS.Output[i][out] = Output[index][out];
            }
        }
        return newDS;
    }

    void NormalizeSetMean(){
        auto maxIndex = argmax(Input);
        auto minIndex = argmin(Input);
        auto maxVal = Input[int(maxIndex/Input.columns())][int(maxIndex%Input.columns())];
        auto minVal = Input[int(minIndex/Input.columns())][int(minIndex%Input.columns())];

        auto minMax = maxVal - minVal;
        for(size_t i = 0; i < Input.columns(); i++){
            for(size_t j = 0; j < Input.rows(); j++){
                Input[i][j] = (Input[i][j]-minVal)/minMax;
            }
        }
    }
    void NormalizeRowMean(){
        auto means = MatrixDS<double>(0, Input.rows(), 1);
        for(size_t i = 0; i < Input.rows(); ++i) {
            for(size_t j = 0; j < Input.columns(); ++j) {
                means[i][0] += Input[i][j];
            }
            means[i][0] /= Input.columns();
        }
        auto stds = MatrixDS<double>(0, Input.rows(), 1);
        for(size_t i = 0; i < Input.rows(); ++i) {
            for(size_t j = 0; j < Input.columns(); ++j) {
                stds[i][0] += pow(Input[i][j] - means[i][0], 2);
            }
            stds[i][0] /= Input.columns();
        }
        for(size_t i = 0; i < Input.rows(); ++i) {
            for(size_t j = 0; j < Input.columns(); ++j) {
                Input[i][j] = (Input[i][j] - means[i][0]) / stds[i][0];
            }
        }
    }

    void NormalizeColMean(){
        Input = Input.Transpose();
        auto means = MatrixDS<double>(0, Input.rows(), 1);
        for(size_t i = 0; i < Input.rows(); ++i) {
            for(size_t j = 0; j < Input.columns(); ++j) {
                means[i][0] += Input[i][j];
            }
            means[i][0] /= Input.columns();
        }
        auto stds = MatrixDS<double>(0, Input.rows(), 1);
        for(size_t i = 0; i < Input.rows(); ++i) {
            for(size_t j = 0; j < Input.columns(); ++j) {
                stds[i][0] += pow(Input[i][j] - means[i][0], 2);
            }
            stds[i][0] /= Input.columns();
        }
        for(size_t i = 0; i < Input.rows(); ++i) {
            for(size_t j = 0; j < Input.columns(); ++j) {
                Input[i][j] = (Input[i][j] - means[i][0]) / stds[i][0];
            }
        }
        Input = Input.Transpose();
    }

    void Normalize(NormalizerMethod method = NormalizerMethod::SET_MEAN){
        switch(method) {
            case SET_MEAN: NormalizeSetMean(); break;
            case COL_MEAN: NormalizeColMean(); break;
            case ROW_MEAN: NormalizeRowMean(); break;
        }
    }

    void Shuffle(){

    }
};

/**
 *
 */
class DataSet {
    friend class ImageDataSet;
public:
    DataSet(const char* filePath, size_t inputCount, size_t outputCount)
    : InputCount(inputCount)
    , OutputCount(outputCount)
    , Training(format("%s%s", filePath, "training.dat").c_str(), inputCount, outputCount)
    , Validation(format("%s%s", filePath, "validation.dat").c_str(), inputCount, outputCount)
    , Test(format("%s%s", filePath, "test.dat").c_str(), inputCount, outputCount)
    { }

    DataSet(size_t inputCount, size_t outputCount)
    : InputCount(inputCount)
    , OutputCount(outputCount)
    , Training(inputCount, outputCount)
    , Validation(inputCount, outputCount)
    , Test(inputCount, outputCount)
    {

    }

    virtual void PrepareDirectory([[maybe_unused]] const char* filePath){

    }

public:
    size_t InputCount;
    size_t OutputCount;

    Set Training;
    Set Validation;
    Set Test;

    int maxEpoch = 1000;
    double stopThreshold = 0.001;
    double eta = 0.0051;
    int batchSize = 5;

    bool verbose = false;
};
