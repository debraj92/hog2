//
// Created by Debraj Ray on 2022-05-30.
//

#include "DQNNet.h"

#include <utility>
#include <iostream>
#include "../plot/pbPlots.hpp"
#include "../plot/supportLib.hpp"

using namespace std;

DQNNet::DQNNet(int inputSize, int outputSize, int hiddenLayer1Size, int hiddenLayer2Size, double learning_rate, const std::string& module_name) {
    cout<<"Creating DQNNet "<<module_name<<endl;

    model = nn::Sequential(nn::Linear(inputSize, hiddenLayer1Size),
                           nn::Sigmoid(),
                           nn::Linear(hiddenLayer1Size, outputSize));

    register_module(module_name, model);

    optimizer = std::make_unique<optim::Adam>(model->parameters(), torch::optim::AdamOptions(learning_rate));
}

Tensor DQNNet::forwardPass1(const Tensor& inputs) {
    cout<<"DQNNet::forwardPass1"<<endl;
    optimizer->zero_grad();
    return model->forward(inputs);
}

Tensor DQNNet::forwardPass2(const Tensor& input1, const Tensor& input2) {
    cout<<"DQNNet::forwardPass2"<<endl;
    return sigmoidLayer(input1 + input2);
}

void DQNNet::saveModel(string &file) {
    cout<<"DQNNet::saveModel"<<endl;
    torch::save(model, file + "/model.pt");
}

void DQNNet::loadModel(string &file) {
    cout<<"DQNNet::loadModel"<<endl;
    torch::load(model, file + "/model.pt");
}

double DQNNet::computeLossAndBackPropagate(Tensor expected, Tensor predicted) {
    cout<<"computeLossAndBackPropagate "<<endl;

    auto lossfn = nn::SmoothL1Loss();

    auto loss = lossfn(predicted, expected);
    cout<<"Network Loss: "<<loss<<endl;
    double loss_value = loss.data().detach().item<double>();
    losses.push_back(loss_value);
    loss_count.push_back(count++);

    loss.backward();
    optimizer->step();

    return loss_value;

}

void DQNNet::saveModel(stringstream &stream) {
    cout<<"DQNNet::saveModel stream"<<endl;
    torch::save(model, stream);
}

void DQNNet::loadModel(stringstream &stream) {
    cout<<"DQNNet::loadModel stream"<<endl;
    torch::load(model, stream);
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

