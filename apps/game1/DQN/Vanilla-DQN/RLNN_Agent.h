//
// Created by Debraj Ray on 2022-05-30.
//

#ifndef EXAMPLE_TESTS_RLNN_AGENT_H
#define EXAMPLE_TESTS_RLNN_AGENT_H

#include "DQNNet.h"
#include "../../gameConstants.h"
#include "../DQN_interface.h"
#include "../ReplayMemory.h"
#include <testing.h>

using namespace std;

class RLNN_Agent : public DQN_interface {

    const LOG_LEVEL LogLevel = LOG_LEVEL::INFO;
    std::unique_ptr<Logger> logger;

    // Hyperparameters
    const double lr = 1e-3;
    // discount factor
    const double gamma = 0.9;
    double epsilon = 1;
    const double epsilon_min = 0.01;
    const double epsilon_decay = 0.998;
    const double alpha = 1;
    const int epsilon_annealing_percent = 60;
    int batchSize = 4000;

    unique_ptr<DQNNet> policyNet;
    unique_ptr<DQNNet> targetNet;
    unique_ptr<DQNNet> policyNetSaved;

    ReplayMemory memory;
    bool startEpsilonDecay;

    std::mutex safeActionSelectionAndTraining;

    bool isExplore(int episodeCount);

    void savePolicyNet();

public:

    RLNN_Agent() {
        policyNet = std::make_unique<DQNNet>(lr, "policyNet");
        targetNet = std::make_unique<DQNNet>(lr, "targetNet");
        policyNetSaved = std::make_unique<DQNNet>(lr, "policyNetSaved");
        // since no learning is performed on the target net
        targetNet->eval();
        policyNetSaved->eval();
        if (!isTrainingMode) {
            policyNet->eval();
        }
        startEpsilonDecay = false;
        logger = std::make_unique<Logger>(LogLevel);
    }

    void setTrainingMode(bool value);

    int selectAction(const observation& currentState, int episodeCount, bool *explore);

    double learnWithDQN();

    // TODO: Interface changes in other DQNs
    void memorizeExperienceForReplay(observation &current, observation &next, int action, float reward, bool done, bool isExploring);

    /// provide file path
    void saveModel(const string &file);

    /// provide file path
    void loadModel(const string &file);

    void updateTargetNet();

    void decayEpsilon();

    void printAction(int action);

    void plotLosses();

#ifdef TESTING

    /// For testing only
    int seedAction = 1;
    int seedExplore = 2;

    virtual ReplayMemory* getAccessToReplayMemory() {
        return &memory;
    }

#endif
};


#endif //EXAMPLE_TESTS_RLNN_AGENT_H
