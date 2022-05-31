//
// Created by Debraj Ray on 2022-05-30.
//

#ifndef EXAMPLE_TESTS_DQNNET_H
#define EXAMPLE_TESTS_DQNNET_H

#include <torch/torch.h>
#include <vector>
#include <string>

using namespace torch;
using namespace std;

class DQNNet : public nn::Module {

    nn::Sequential model;
    int inputSize;
    int outputSize;
    int hiddenLayer1Size;
    int hiddenLayer2Size;

    optim::Adam optimizer;

public:
    DQNNet(int inputSize, int outputSize, int hiddenLayer1Size, int hiddenLayer2Size, double learning_rate, const std::string& module_name);

    Tensor forward(const Tensor& inputs);

    void saveModel(string &file);

    void loadModel(string &file);

    void saveModel(stringstream &stream);

    void loadModel(stringstream &stream);

    void computeLossAndBackPropagate(Tensor expected, Tensor predicted);
};


#endif //EXAMPLE_TESTS_DQNNET_H
