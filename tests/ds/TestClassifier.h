#pragma once

#include "../Test.h"
#include <cassert>
#include <math/ds/MatrixDS.h>
#include <math/ds/NN.h>


bool TestClassifier() {
    NN<4, 3> nn(2);
    nn.SetLayers(7);
    DataSet training("../../resources/iris_data_files/training.dat", 4, 3);
    nn.Train(training);
    nn.Train(training);
    nn.Train(training);
    nn.Train(training);

    NN<3, 1> nn2(5);
    nn2.SetLayers(5);
    DataSet training2("../../resources/simple_ds/training.dat", 3, 1);
    nn2.Train(training2, 1000, 0.01, 0.001);
    nn2.Train(training2, 1000, 0.01, 0.001);
    nn2.Train(training2, 1000, 0.01, 0.001);
    nn2.Train(training2, 1000, 0.01, 0.001);
    nn2.Train(training2, 1000, 0.01, 0.001);

    NN<10, 2> nn3(7);
    nn3.SetLayers(10);
    DataSet training3("../../resources/cancer/set1/training.dat", 10, 2);
    nn3.Train(training3, 100);
    nn3.Train(training3, 100);
    nn3.Train(training3, 100);
    nn3.Train(training3, 100);
    return true;
}