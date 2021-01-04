#pragma once

#include "../Test.h"
#include <cassert>
#include <math/ds/MatrixDS.h>
#include <math/ds/NN.h>


bool TestClassifier() {
    NN nn;
    nn.SetLayers({{784, 30},{30, 10}});
    ImageDataSet ds(784, 10);

    nn.SGD(ds, 30, 10, 3.0);
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