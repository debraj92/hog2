//
// Created by Debraj Ray on 2022-05-30.
//

#include "DQNNet.h"

#include <utility>
#include <iostream>
#include "../../plot/pbPlots.hpp"
#include "../../plot/supportLib.hpp"

using namespace std;

DQNNet::DQNNet(int inputSize, int outputSize, int hiddenLayer1Size, int hiddenLayer2Size, double learning_rate, const std::string& module_name) : m_value(hiddenLayer1Size, 1),
                                                                                                                                                  m_advantage(hiddenLayer1Size, outputSize)
{
    cout<<"Creating DQNNet "<<module_name<<endl;

    m_sequential = nn::Sequential(nn::Linear(inputSize, hiddenLayer1Size),
                                  nn::Sigmoid());

    register_module(module_name + "_primary", m_sequential);
    register_module(module_name + "_secondary_value", m_value);
    register_module(module_name + "_secondary_advantage", m_advantage);

    optimizer = std::make_unique<optim::Adam>(this->parameters(), torch::optim::AdamOptions(learning_rate));
}

Tensor DQNNet::forwardPass(const Tensor& inputs) {
    cout<<"DQNNet::forwardPass"<<endl;
    optimizer->zero_grad();
    return m_sequential->forward(inputs);
}

Tensor DQNNet::forwardPassValue(const Tensor &inputs) {
    return m_value(inputs);
}

Tensor DQNNet::forwardPassAdvantage(const Tensor &inputs) {
    return m_advantage(inputs);
}

void DQNNet::saveModel(string &file) {
    cout<<"DQNNet::saveModel"<<endl;
    torch::save(m_sequential, file + "/m_sequential.pt");
    torch::save(m_value, file + "/m_value.pt");
    torch::save(m_advantage, file + "/m_adv.pt");
}

void DQNNet::loadModel(string &file) {
    cout<<"DQNNet::loadModel"<<endl;
    torch::load(m_sequential, file + "/m_sequential.pt");
    torch::load(m_value, file + "/m_value.pt");
    torch::load(m_advantage, file + "/m_adv.pt");
}


double DQNNet::computeLossAndBackPropagate(const Tensor& expected, const Tensor& predicted) {
    cout<<"computeLossAndBackPropagate "<<endl;

    auto lossfn = nn::SmoothL1Loss();

    auto loss = lossfn(predicted, expected);
    cout<<"Network Loss: "<<loss<<endl;
    double loss_value = loss.data().detach().item<double>();
    losses.push_back(loss_value);
    loss_count.push_back(count++);

    loss.backward();

    /// Gradient clipping
    for (auto &param: this->parameters()) {
        param.grad().data().clamp_(-1, 1);
    }

    optimizer->step();

    return loss_value;

}


void DQNNet::plotLoss() {
    cout<<"Network Loss"<<endl;
    /*for(double loss: losses) {
        cout<<loss<<endl;
    }*/
    RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();
    StringReference *errorMessage = new StringReference();
    auto success = DrawScatterPlot(imageReference, 1000, 1000, &loss_count, &losses, errorMessage);
    if(success){
        vector<double> *pngdata = ConvertToPNG(imageReference->image);
        WriteToFile(pngdata, "/Users/debrajray/MyComputer/RL-A-STAR-THESIS/plot/loss_output.png");
        DeleteImage(imageReference->image);
    }else{
        cerr << "Error: ";
        for(wchar_t c : *errorMessage->string){
            wcerr << c;
        }
        cerr << endl;
    }

}

void DQNNet::loadModel(stringstream &stream, const DQNNet::MODEL_TYPE &model_type) {
    switch(model_type) {
        case SEQUENTIAL:
            torch::load(m_sequential, stream);
            break;
        case VALUE:
            torch::load(m_value, stream);
            break;
        case ADVANTAGE:
            torch::load(m_advantage, stream);
            break;
    }
}

void DQNNet::saveModel(stringstream &stream, const DQNNet::MODEL_TYPE &model_type) {
    switch(model_type) {
        case SEQUENTIAL:
            torch::save(m_sequential, stream);
            break;
        case VALUE:
            torch::save(m_value, stream);
            break;
        case ADVANTAGE:
            torch::save(m_advantage, stream);
            break;
    }
}


