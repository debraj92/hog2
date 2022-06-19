//
// Created by Debraj Ray on 2022-05-28.
//

#ifndef EXAMPLE_TESTS_REPLAYMEMORY_H
#define EXAMPLE_TESTS_REPLAYMEMORY_H

#include <torch/torch.h>
#include <memory>
#include <vector>
#include <iostream>


#include <algorithm>
#include <iterator>
#include <random>
#include <vector>

#include "../observation.h"
#include "../gameConstants.h"

using namespace std;
class ReplayMemory {

    const LOG_LEVEL LogLevel = LOG_LEVEL::INFO;
    std::unique_ptr<Logger> logger;

    vector<vector<float>> buffer_states;
    vector<vector<float>> buffer_next_states;
    vector<long> buffer_actions;
    vector<float> rewards;
    vector<bool> dones;
    int idx = 0;

    bool isBufferFull = false;

    double exploitation_window_start = MIN_EXPLOITATION_WINDOW_START_FOR_MEMORY;

    void storeExperience(observation &current, observation &next, int action, float reward, bool done);

public:

    torch::Tensor tensor_states;
    torch::Tensor tensor_next_states;
    torch::Tensor tensor_actions;
    torch::Tensor tensor_rewards;
    torch::Tensor tensor_dones;

    ReplayMemory() : buffer_states(MAX_CAPACITY_REPLAY_BUFFER, vector<float>(MAX_ABSTRACT_OBSERVATIONS, 0)),
                     buffer_next_states(MAX_CAPACITY_REPLAY_BUFFER, vector<float>(MAX_ABSTRACT_OBSERVATIONS, 0)),
                     buffer_actions(MAX_CAPACITY_REPLAY_BUFFER),
                     rewards(MAX_CAPACITY_REPLAY_BUFFER),
                     dones(MAX_CAPACITY_REPLAY_BUFFER)
    {
        logger = std::make_unique<Logger>(LogLevel);
        //logger->openLogFile();
    }

    ~ReplayMemory() {
        //logger->closeLogFile();
    }
    void sampleBatch(int batchSize);

    void storeExperience(observation &current, observation &next, int action, float reward, bool done, bool isExploring);

    int getBufferSize();

    void logStateVector(observation &ob);

};


#endif //EXAMPLE_TESTS_REPLAYMEMORY_H
