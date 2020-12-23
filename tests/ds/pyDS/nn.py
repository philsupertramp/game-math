import tensorflow as tf
from tensorflow import keras

class Linear(keras.layers.Layer):
    def __init__(self, units=32, input_dim=32):
        super(Linear, self).__init__()
        w_init = tf.random_normal_initializer()
        self.w = tf.Variable(
            initial_value=w_init(shape=(input_dim, units), dtype="float32"),
            trainable=True,
        )
        b_init = tf.ones_initializer()
        self.b = tf.Variable(
            initial_value=b_init(shape=(units,), dtype="float32"), trainable=True
        )

    def call(self, inputs):
        print(tf.matmul(inputs, self.w))
        print(self.b)
        print(tf.matmul(inputs, self.w) + self.b)
        return tf.matmul(inputs, self.w) + self.b


x = tf.ones((15, 5))
linear_layer = Linear(4, 5)
y = linear_layer(x)
