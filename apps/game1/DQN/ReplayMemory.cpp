//
// Created by Debraj Ray on 2022-05-28.
//

#include "ReplayMemory.h"

#include <iostream>

#include <xtensor/xrandom.hpp>

using namespace xt;
using namespace std;

void ReplayMemory::sampleBatch(const int batchSize) {
    cout<<"ReplayMemory::sampleBatch"<<endl;
    if (buffer_states.size() == 0) {
        return;
    }
    random::seed(time(nullptr));
    auto random_indices = random::randint<int>({batchSize}, 0, getBufferSize() - 1, random::get_default_random_engine());

    vector<long> temp_actions;
    vector<long> temp_rewards;
    vector<long> temp_dones;
    vector<torch::Tensor> stateSequence(batchSize);
    vector<torch::Tensor> nextStateSequence(batchSize);

    auto options = torch::TensorOptions().dtype(torch::kFloat32).device(torch::kCPU);
    auto options2 = torch::TensorOptions().dtype(torch::kLong).device(torch::kCPU);

    tensor_states = torch::zeros({batchSize, MAX_ABSTRACT_OBSERVATIONS}, options);
    tensor_next_states = torch::zeros({batchSize, MAX_ABSTRACT_OBSERVATIONS}, options);

    tensor_states = torch::empty({batchSize, MAX_ABSTRACT_OBSERVATIONS}, options);

    int i = 0;
    for(int random_index: random_indices) {
        temp_actions.emplace_back(buffer_actions[random_index]);
        temp_rewards.emplace_back(rewards[random_index]);
        temp_dones.emplace_back(dones[random_index] ? 1 : 0);
        tensor_states.slice(0, i,i+1) = torch::from_blob(buffer_states[random_index].data(), {MAX_ABSTRACT_OBSERVATIONS}, options);
        tensor_next_states.slice(0, i,i+1) = torch::from_blob(buffer_next_states[random_index].data(), {MAX_ABSTRACT_OBSERVATIONS}, options);
        i++;
    }

    tensor_actions = torch::from_blob(temp_actions.data(), {batchSize}, options2).clone();
    tensor_rewards = torch::from_blob(temp_rewards.data(), {batchSize}, options2).clone();
    tensor_dones = torch::from_blob(temp_dones.data(), {batchSize}, options2).clone();
}

void ReplayMemory::storeExperience(observation &current, observation &next, int action, int reward, bool done) {
    cout<<"ReplayMemory::storeExperience"<<endl;
    float observation_vector[MAX_ABSTRACT_OBSERVATIONS] = {0};
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

int ReplayMemory::getBufferSize() {
    if (buffer_states.size() <= MAX_CAPACITY_REPLAY_BUFFER) {
        return idx + 1;
    }
    return MAX_CAPACITY_REPLAY_BUFFER;
}

