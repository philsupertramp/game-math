#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tuesday Oct 2, 2018
@author: Madhuri Suthar, PhD Candidate in Electrical and Computer Engineering, UCLA
"""

# Imports
import numpy as np

# Each row is a training example, each column is a feature  [X1, X2, X3]
X=np.array(([0,0,1],[0,1,1],[1,0,1],[1,1,1]), dtype=float)
y=np.array(([0],[1],[1],[0]), dtype=float)

# Define useful functions    

# Activation function
def sigmoid(t):
    return 1/(1+np.exp(-t))

# Derivative of sigmoid
def sigmoid_derivative(p):
    return p * (1 - p)


# Class definition
class NeuralNetwork:
    def __init__(self, trainingInput, validationOutput, layers = 2):
        self.trainingInput = trainingInput
        self.weights = np.random.rand(self.trainingInput.shape[1], 2)  # considering we have 4 nodes in the hidden layer
        self.bias = np.random.rand(2, 1)
        self.validationOutput = validationOutput
        self.output = np. zeros(validationOutput.shape)
        self.num_layers = layers
        self.layers = [None for _ in range(self.num_layers)]
        self.output_layer = None

    def feedforward(self):
        layer = np.zeros((self.weights.shape[1]))
        for index in range(self.num_layers):
            newLayer = sigmoid(np.dot(self.trainingInput, self.weights))
            self.layers[index] = newLayer
            layer = layer + newLayer
        self.output_layer = sigmoid(np.dot(layer, self.bias))
        return self.output_layer

    def backprop(self, eta=2.0):
        for index in range(len(self.layers)):
            d_bias = np.dot(self.layers[index].T, eta*(self.validationOutput -self.output)*sigmoid_derivative(self.output))
            d_weights = np.dot(self.trainingInput.T, np.dot(eta*(self.validationOutput -self.output)*sigmoid_derivative(self.output), self.bias.T)*sigmoid_derivative(self.layers[index]))
            # breakpoint()
            self.weights += d_weights
            self.bias += d_bias

    def train(self):
        self.output = self.feedforward()
        self.backprop(0.2)


NN = NeuralNetwork(X,y, 5)
for i in range(1000): # trains the NN 1,000 times
    if i % 100 ==0:
        print ("for iteration # " + str(i) + "\n")
        print ("Input : \n" + str(X))
        print ("Actual Output: \n" + str(y))
        out = NN.feedforward()
        print ("Predicted Output: \n" + str(out))
        print ("Loss: \n" + str(np.mean(np.square(y - out)))) # mean sum squared loss
        print ("\n")

    NN.train()

print(NN.weights)
