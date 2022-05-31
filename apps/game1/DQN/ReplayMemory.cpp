//
// Created by Debraj Ray on 2022-05-28.
//

#include "ReplayMemory.h"

#include <iostream>

#include <xtensor/xrandom.hpp>

using namespace xt;
using namespace std;

void ReplayMemory::sampleBatch(int batchSize) {

    if (buffer_states.size() == 0) {
        return;
    }
    random::seed(time(NULL));
    auto random_indices = random::randint<int>({batchSize}, 0, buffer_states.size()-1, random::get_default_random_engine());

    double temp_actions[batchSize];
    double temp_rewards[batchSize];
    batch_dones.clear();

    auto options = torch::TensorOptions().dtype(torch::kFloat64).device(torch::kCUDA);
    auto options2 = torch::TensorOptions().dtype(torch::kInt64).device(torch::kCUDA);

    tensor_states = torch::zeros({batchSize, MAX_ABSTRACT_OBSERVATIONS}, options);
    tensor_next_states = torch::zeros({batchSize, MAX_ABSTRACT_OBSERVATIONS}, options);

    int i = 0;
    //vector<int> done_indices;
    for(int random_index: random_indices) {
        temp_actions[i] = buffer_actions[random_index];
        temp_rewards[i] = rewards[random_index];
        if(dones[random_index]) {
            batch_dones.push_back(torch::tensor({random_index}, options2));
        }
        tensor_states.slice(0, i,i+1) = torch::from_blob(buffer_states[i].data(), {MAX_ABSTRACT_OBSERVATIONS}, options);
        tensor_next_states.slice(0, i,i+1) = torch::from_blob(buffer_next_states[i].data(), {MAX_ABSTRACT_OBSERVATIONS}, options);
        i++;
    }

    tensor_actions = torch::from_blob(temp_actions, {batchSize}, options);
    tensor_rewards = torch::from_blob(temp_rewards, {batchSize}, options);

}

void ReplayMemory::storeExperience(observation current, observation next, int action, int reward, bool done) {

    double observation_vector[MAX_ABSTRACT_OBSERVATIONS];
    current.flattenObservationToVector(observation_vector);
    buffer_states[idx].assign(observation_vector, observation_vector + MAX_ABSTRACT_OBSERVATIONS);
    fill_n(observation_vector, MAX_ABSTRACT_OBSERVATIONS, 0);
    next.flattenObservationToVector(observation_vector);
    buffer_next_states[idx].assign(observation_vector, observation_vector + MAX_ABSTRACT_OBSERVATIONS);
    buffer_actions[idx] = action;
    rewards[idx] = reward;
    dones[idx] = done;
    idx = (idx + 1) % MAX_CAPACITY_REPLAY_BUFFER;

}

