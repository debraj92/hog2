//
// Created by Debraj Ray on 2022-05-28.
//

#include "ReplayMemory.h"

#include <iostream>

#include <xtensor/xrandom.hpp>

using namespace xt;
using namespace std;

void ReplayMemory::sampleBatch(const int batchSize) {
    logger->logDebug("ReplayMemory::sampleBatch")->endLineDebug();
    if ((not isBufferFull) and idx == 0) {
        return;
    }
    random::seed(time(nullptr));
    auto random_indices = random::randint<int>({batchSize}, 0, getBufferSize() - 1, random::get_default_random_engine());

    vector<long> temp_actions;
    vector<float> temp_rewards;
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
    tensor_rewards = torch::from_blob(temp_rewards.data(), {batchSize}, options).clone();
    tensor_dones = torch::from_blob(temp_dones.data(), {batchSize}, options2).clone();
}

void ReplayMemory::storeExperience(observation &current, observation &next, int action, float reward, bool done) {
    if((not isBufferFull) and (idx + 1) == MAX_CAPACITY_REPLAY_BUFFER) {
        isBufferFull = true;
    }
    logger->logDebug("ReplayMemory::storeExperience")->endLineDebug();
    //logStateVector(current);
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
    if (not isBufferFull) {
        return idx;
    }
    return MAX_CAPACITY_REPLAY_BUFFER;
}

void ReplayMemory::storeExperience(observation &current, observation &next, int action, float reward, bool done,
                                   bool isExploring) {
    if(isExploring) {
        storeExperience(current, next, action, reward, done);
        return;
    }

    /// Case when this action is due to exploitation
    /**
     * Check if a random number generated from an exponential distribution is within a moving sliding window
     */
    double const exp_dist_mean   = 1;
    double const exp_dist_lambda = 1 / exp_dist_mean;

    std::random_device rd;

    std::exponential_distribution<> rng (exp_dist_lambda);
    std::mt19937 rnd_gen (rd ());
    double random_number = rng (rnd_gen);
    if (random_number > exploitation_window_start) {
        logger->logDebug("Saving Exploitation")->endLineDebug();
        storeExperience(current, next, action, reward, done);
    }

    exploitation_window_start += (10.00 - MIN_EXPLOITATION_WINDOW_START_FOR_MEMORY) / MAX_EPISODES;
}

void ReplayMemory::logStateVector(observation &ob) {
    string state="(" + to_string(ob.playerX) + "," + to_string(ob.playerY) +") dir: ";
    if (ob.direction == N) {
        state += "N";
    }
    if (ob.direction == S) {
        state += "S";
    }
    if (ob.direction == E) {
        state += "E";
    }
    if (ob.direction == W) {
        state += "W";
    }
    if (ob.direction == NE) {
        state += "NE";
    }
    if (ob.direction == NW) {
        state += "NW";
    }
    if (ob.direction == SE) {
        state += "SE";
    }
    if (ob.direction == SW) {
        state += "SW";
    }
    state += " tra: ";
    if(ob.trajectory_off_track) {
        state += "off";
    }
    if (ob.trajectory_on_track) {
        state += "on";
    }
    if (ob.trajectory_front) {
        state += "F";
    }
    if (ob.trajectory_left) {
        state += "L";
    }
    if (ob.trajectory_right) {
        state += "R";
    }
    state += " obs: ";
    if(ob.obstacle_front <= VISION_RADIUS) {
        state += "F |";
    }
    if(ob.obstacle_front_left <= VISION_RADIUS) {
        state += "FL |";
    }
    if (ob.obstacle_front_right <= VISION_RADIUS) {
        state += "FR |";
    }
    if (ob.obstacle_left <= VISION_RADIUS) {
        state += "L |";
    }
    if (ob.obstacle_right <= VISION_RADIUS) {
        state += "R";
    }
    state += " Enmy: ";
    if (ob.enemy_distance_1 < MAX_DISTANCE) {
        state += "1. D:"+to_string(ob.enemy_distance_1)+" A:"+to_string(ob.enemy_angle_1 * 10) + " | ";
    }
    if (ob.enemy_distance_2 < MAX_DISTANCE) {
        state += "2. D:"+to_string(ob.enemy_distance_2)+" A:"+to_string(ob.enemy_angle_2 * 10) + " | ";
    }
    if (ob.enemy_distance_3 < MAX_DISTANCE) {
        state += "3. D:"+to_string(ob.enemy_distance_3)+" A:"+to_string(ob.enemy_angle_3 * 10) + " | ";
    }
    logger->logToFileInfo(state)->endLineInfoFile();
}

