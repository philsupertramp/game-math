#include "../Test.h"
#include <math/ds/SequentialModel.h>
#include <math/ds/layers/ConvolutionalLayer.h>
#include <math/ds/layers/DenseLayer.h>
#include <math/ds/layers/FlattenLayer.h>
#include <math/ds/layers/PoolingLayer.h>
#include <math/ds/layers/RescaleLayer.h>
#include <math/Matrix.h>

/**
 * Example to write matrix to image:

#include <Magick++.h>
#include <Magick++/Image.h>
template<typename T>
void Render(const Matrix<T>& A, const char* filename){
    Magick::Image image;
    image.verbose(true);
    std::string type;
    switch(A.elements()){
        case 1: type = "K"; break;
        case 2: type = "CY"; break;
        case 3: type = "RGB"; break;
        case 4: type = "RGBA"; break;
    }
    image.read(A.rows(), A.columns(), type.c_str(), Magick::FloatPixel,(void*) &A);
    image.scale("600x600");
    image.write(filename);
}

 */
class SequentialModelTestCase : public Test
{
    /**
     * This test implements an example VGG-16 model
     * to classify image data.
     * It consists of a SequentialModel which compiles several layers
     * and uses SGD (statistic gradient decent) method for model fitting
     * @return
     */
    bool TestVGG_16_Model() {
        int numClasses = 5;
        auto model     = SequentialModel<double>({
        std::make_shared<Preprocessing::RescaleLayer>(1. / 255),
        std::make_shared<ConvolutionalLayer<double>>(16, 3),
        std::make_shared<PoolingLayer<double>>(3, 2),
        std::make_shared<ConvolutionalLayer<double>>(32, 3),
        std::make_shared<PoolingLayer<double>>(3, 2),
        std::make_shared<ConvolutionalLayer<double>>(64, 3),
        std::make_shared<PoolingLayer<double>>(3, 2),
        std::make_shared<FlattenLayer<double>>(),
        std::make_shared<DenseLayer<double>>(128, ActivationFunctions::RELU),
        std::make_shared<DenseLayer<double>>(numClasses),
        });

        model.Compile();
        model.Summary();

//        model.Train();
        Matrix<double> B(0, 180, 180, 3);
        Tensor<double> tensor(1, 3);
        tensor.push_back(B);
//        auto res = model.Predict(tensor);
//        std::cout << B << res[0];

        return true;
    }

public:
    virtual void run() override { TestVGG_16_Model(); }
};


int main() {
    SequentialModelTestCase().run();
    return 0;
}