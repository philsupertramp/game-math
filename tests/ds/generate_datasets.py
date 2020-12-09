#!/bin/env/python
import math
import random


class DataSet:
    def __init__(self, input_count, output_count, target_method):
        self.Training = dict()
        self.Validation = dict()
        self.Test = dict()
        self.target_method = target_method
        self.input_count = input_count
        self.output_count = output_count

    def generate(self, train_count, validation_count, test_count):
        self.Training['input'] = list()
        self.Training['output'] = list()
        self.Validation['input'] = list()
        self.Validation['output'] = list()
        self.Test['input'] = list()
        self.Test['output'] = list()
        index = 0
        i = -1.
        stepwidth = 2./train_count
        while index < train_count:
            self.Training['input'].append(i)
            self.Training['output'].append(self.target_method(self.Training['input'][index]))
            i += stepwidth
            index += 1

        index = 0
        i = -1.
        stepwidth = 2./validation_count
        while index < validation_count:
            self.Validation['input'].append(-1.0 + random.random() * 2.)
            self.Validation['output'].append(self.target_method(self.Validation['input'][index]))
            i += stepwidth
            index += 1

        index = 0
        i = -1.
        stepwidth = 2./test_count
        while index < test_count:
            self.Test['input'].append(-1.0 + random.random() * 2.)
            self.Test['output'].append(self.target_method(self.Test['input'][index]))
            i += stepwidth
            index += 1

    def write(self, path):
        with open(f'{path}/training.dat', 'w') as f:
            for i in range(len(self.Training['input'])):
                f.write(f'{self.Training["input"][i]}\t{self.Training["output"][i]}\n')

        with open(f'{path}/validation.dat', 'w') as f:
            for i in range(len(self.Validation['input'])):
                f.write(f'{self.Validation["input"][i]}\t{self.Validation["output"][i]}\n')

        with open(f'{path}/test.dat', 'w') as f:
            for i in range(len(self.Test['input'])):
                f.write(f'{self.Test["input"][i]}\t{self.Test["output"][i]}\n')


if __name__ == '__main__':
    ds = DataSet(1, 1, lambda x: x * x)
    ds.generate(360, 150, 140)
    ds.write('.')
