#pragma once

#include <Magick++.h>
#include <Magick++/Image.h>
#include <filesystem>
#include <fstream>
#include <map>
struct Set {

    MatrixDS<double> Input;
    MatrixDS<double> Output;
    size_t InputCount;
    size_t OutputCount;
    size_t count = 0;

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
                        Output[count][i - InputCount] = std::atoi(val.c_str());
                    }
                }
                ++count;
            }
            dataFile.close();
            //            Classes = OutputToClass(Output);
        } else {
            std::cerr << "Unable to open file";
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

class ImageDataSet
: public DataSet
{
public:

    ImageDataSet(size_t inputCount, size_t outputCount)
    : DataSet(inputCount, outputCount)
    { }

    /**
     * @param filePath
     */
    virtual void PrepareDirectory(const char* filePath) override {
        /** identify "classes" as files in nested directories */

        std::cout << "Moving files..." << std::flush;
        std::filesystem::remove_all(trainDirectory);
        std::filesystem::copy(
        filePath,
        trainDirectory,
        std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive);

        totalCount = 0;
        size_t classCount = 0;
        classNames.clear();
        for(const auto& entry : std::filesystem::directory_iterator(trainDirectory)) {
            if(entry.is_directory()) {
                classNames.push_back(entry.path().filename());
                auto dirIter =  std::filesystem::directory_iterator(entry);
                totalCount += std::count_if(begin(dirIter), end(dirIter), [](auto& elem) { return elem.is_regular_file(); });
                classCount += 1;
            }
        }
        trainingCount = (int)(totalCount * 0.8);
        validationCount = (int)(totalCount - trainingCount);
        Training.Input = MatrixDS<double>(0, trainingCount, InputCount);
        Training.Output = MatrixDS<double>(0, trainingCount, OutputCount);
        Validation.Input = MatrixDS<double>(0, validationCount, InputCount);
        Validation.Output = MatrixDS<double>(0, validationCount, OutputCount);

        size_t index = 0;
        for(const auto& entry : std::filesystem::directory_iterator(trainDirectory)) {
            if(entry.is_directory()) {
                MatrixDS<bool> elem(false, 1, classCount);
                elem[0][index] = true;
                classes.push_back(elem);
                index += 1;
            }
        }
        std::cout << "done!\nDirectory \"" << trainDirectory << "\" ready." << std::endl;
         std::cout << "Found " << totalCount
                  << " files belonging to " << classCount
                  << " classes.\nUsing " << validationCount
                  << " files for validation\n" << std::flush;
    }

    /**
     * loads data set into memory
     *
     *
     * we split the dataset into
     * - 80% training
     * - 20% validation
     * -  0% test
     *
     * Example: 100 images => 80 training, 20 validation, 0 test
     */
    void Cache(){
        std::cout << "Resizing files and saving into memory..." << std::flush;
        size_t count = 0, i = 0, classCount = 0;
        size_t trainingIter = 0, validationIter = 0;
        for(const auto& classDirectory : std::filesystem::directory_iterator(trainDirectory)) {
            bool hadEntry = false;
            if(!classDirectory.is_directory() || classCount >= classes.size()) continue;
            for(const auto& entry : std::filesystem::directory_iterator(classDirectory)) {
                if(!entry.is_regular_file()) continue;

                hadEntry = true;
                Magick::Image image;
                image.verbose(verbose);
                image.magick("JPG");
                image.read(std::string(entry.path()));
                // PZ: we don't scale but resample, to enforce dimension width * height and ignore loss in content
                image.resample(Magick::Point(imageWidth, imageHeight));
                Set* target;
                bool is_validation = (trainingIter >= trainingCount || ((count % (int)totalCount * (validationShare)) == 0 && count != 0 && validationIter < validationCount));
                target             = is_validation ? &Validation : &Training;
                Magick::PixelData pixelBlob(image, "RGBA", Magick::FloatPixel);

                auto* in = (float*)pixelBlob.data();

                for(i = 0; i < InputCount; i++) {
                    (*target).Input[is_validation ? validationIter : trainingIter][i] = in[i];
                }
                for(i = 0; i < OutputCount; i++) {
                    (*target).Output[is_validation ? validationIter : trainingIter][i] = classes[classCount][0][i];
                }
                if(is_validation) {
                    validationIter += 1;
                } else {
                    trainingIter += 1;
                }
                count += 1;

            }
            classCount += hadEntry;
        }
    }
public:
    size_t imageHeight = 180;
    size_t imageWidth = 180;
    double validationShare = 0.2;
    const char* trainDirectory = "../../resources/image_classification/training/";
    std::vector<MatrixDS<bool>> classes;
    std::vector<std::string> classNames;
    size_t totalCount = 0;
    size_t trainingCount = 0;
    size_t validationCount = 0;

};
