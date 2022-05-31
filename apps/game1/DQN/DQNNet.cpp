//
// Created by Debraj Ray on 2022-05-30.
//

#include "DQNNet.h"

#include <utility>
#include <iostream>

using namespace std;

DQNNet::DQNNet(int inputSize, int outputSize, int hiddenLayer1Size, int hiddenLayer2Size, double learning_rate, const std::string& module_name) : inputSize(inputSize),
                                                                                            outputSize(outputSize),
                                                                                            hiddenLayer1Size(hiddenLayer1Size),
                                                                                            hiddenLayer2Size(hiddenLayer2Size),
                                                                                            optimizer(this->parameters(), torch::optim::AdamOptions(learning_rate)){
    cout<<"Creating DQNNet "<<module_name<<endl;

    model = nn::Sequential(nn::Linear(inputSize, hiddenLayer1Size),
                           nn::ReLU(),
                           nn::Linear(hiddenLayer1Size, hiddenLayer2Size),
                           nn::ReLU(),
                           nn::Linear(hiddenLayer2Size, outputSize),
                           nn::Softmax(1));

    register_module(module_name, model);
}

Tensor DQNNet::forward(const Tensor& inputs) {
    cout<<"DQNNet::forward"<<endl;
    return model->forward(inputs);
}

void DQNNet::saveModel(string &file) {
    cout<<"DQNNet::saveModel"<<endl;
    torch::save(model, file + "/model.pt");
}

void DQNNet::loadModel(string &file) {
    cout<<"DQNNet::loadModel"<<endl;
    torch::load(model, file + "/model.pt");
}

void DQNNet::computeLossAndBackPropagate(Tensor expected, Tensor predicted) {
    optimizer.zero_grad();
    auto lossfn = nn::MSELoss();
    auto meanLoss = lossfn(expected, predicted).mean();
    meanLoss.backward();
    optimizer.step();
}

void DQNNet::saveModel(stringstream &stream) {
    cout<<"DQNNet::saveModel stream"<<endl;
    torch::save(model, stream);
}

void DQNNet::loadModel(stringstream &stream) {
    cout<<"DQNNet::loadModel stream"<<endl;
    torch::load(model, stream);
}
