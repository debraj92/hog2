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

    nn::Sigmoid sigmoidLayer;

    int count = 1;

    unique_ptr<optim::Adam> optimizer;

    vector<double> losses;
    vector<double> loss_count;

public:
    DQNNet(int inputSize, int outputSize, int hiddenLayer1Size, int hiddenLayer2Size, double learning_rate, const std::string& module_name);

    Tensor forwardPass1(const Tensor& inputs);

    Tensor forwardPass2(const Tensor& input1, const Tensor& input2);

    void saveModel(string &file);

    void loadModel(string &file);

    void saveModel(stringstream &stream);

    void loadModel(stringstream &stream);

    double computeLossAndBackPropagate(Tensor expected, Tensor predicted);

    void plotLoss();
};


#endif //EXAMPLE_TESTS_DQNNET_H
