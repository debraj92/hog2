//
// Created by Debraj Ray on 2022-05-30.
//

#ifndef EXAMPLE_TESTS_RLNN_AGENT_H
#define EXAMPLE_TESTS_RLNN_AGENT_H

#include "DQNNet.h"
#include "../gameConstants.h"
#include "ReplayMemory.h"
#include <string>

using namespace std;

class RLNN_Agent {

    // Hyperparameters
    double alpha = 1e-2;
    // discount factor
    double gamma = 0.7;
    double epsilon = 1;
    double epsilon_min = 0.01;
    double epsilon_decay = 0.95;

    int batchSize = 50;

    unique_ptr<DQNNet> policyNet;
    unique_ptr<DQNNet> targetNet;

    ReplayMemory memory;


public:

    RLNN_Agent(bool isTrainingMode) {
        int sizeHiddenLayers = (MAX_ABSTRACT_OBSERVATIONS + ACTION_SPACE) / 2;
        policyNet = std::make_unique<DQNNet>(MAX_ABSTRACT_OBSERVATIONS, ACTION_SPACE, sizeHiddenLayers, sizeHiddenLayers, alpha, "policyNet");
        targetNet = std::make_unique<DQNNet>(MAX_ABSTRACT_OBSERVATIONS, ACTION_SPACE, sizeHiddenLayers, sizeHiddenLayers, alpha, "policyNet");
        // since no learning is performed on the target net
        targetNet->eval();
        if (!isTrainingMode) {
            policyNet->eval();
        }
    }

    int selectAction(observation currentState);

    void learn();

    void saveModel(string &file);

    void loadModel(string &file);

    void updateTargetNet();

    void decayEpsilon();
};


#endif //EXAMPLE_TESTS_RLNN_AGENT_H
