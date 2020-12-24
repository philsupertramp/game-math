#pragma once

#include "../Test.h"
#include <cassert>
#include <math/ds/MatrixDS.h>
#include <math/ds/NN.h>


bool TestClassifier() {
    NN<3, 1> nn;
    DataSet training("../../resources/simple_ds/", 3, 1);
    training.maxEpoch = 100;
    training.eta = 0.02;
    training.batchSize = -1;
    nn.SetSequentialLayers({ {3, 1}});
    nn.Train(training);
//    size_t batchSize = 32;
//    size_t imageHeight = 180;
//    size_t imageWidth = 180;
//    ImageDataSet ds(imageHeight * imageWidth, 1);
//    // get files using  `wget https://storage.googleapis.com/download.tensorflow.org/example_images/flower_photos.tgz`
//    ds.PrepareDirectory("../../resources/image_classification/flower_photos/");
//    ds.Cache();
    /**
     This API would be nice!

     num_classes = 5

        model = Sequential([
            # rescaling
            layers.experimental.preprocessing.Rescaling(1./255, input_shape=(img_height, img_width, 3)),
            layers.Conv2D(16, 3, padding='same', activation='relu'),
          layers.MaxPooling2D(),
          layers.Conv2D(32, 3, padding='same', activation='relu'),
          layers.MaxPooling2D(),
          layers.Conv2D(64, 3, padding='same', activation='relu'),
          layers.MaxPooling2D(),
          layers.Flatten(),
          layers.Dense(128, activation='relu'),
          layers.Dense(num_classes)
        ])

     */

    return true;
}