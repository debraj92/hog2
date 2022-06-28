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
    if ((not isBufferFull) and idx <= 1) {
        return;
    }

#ifdef TESTING
    random::seed(seedSamplingBatch);
#else
    random::seed(time(nullptr));
#endif

    auto random_indices = random::randint<int>({batchSize}, 0, getBufferSize() - 1, random::get_default_random_engine());

    vector<long> temp_actions;
    vector<float> temp_rewards;
    vector<long> temp_dones;

    float obstaclesFOV_current_temp[batchSize][FOV_WIDTH][FOV_WIDTH];
    float enemiesFOV_current_temp[batchSize][FOV_WIDTH][FOV_WIDTH];
    float pathFOV_current_temp[batchSize][FOV_WIDTH][FOV_WIDTH];

    float pathFOV_next_temp[batchSize][FOV_WIDTH][FOV_WIDTH];
    float obstaclesFOV_next_temp[batchSize][FOV_WIDTH][FOV_WIDTH];
    float enemiesFOV_next_temp[batchSize][FOV_WIDTH][FOV_WIDTH];

    auto options = torch::TensorOptions().dtype(torch::kFloat32).device(torch::kCPU);
    auto options2 = torch::TensorOptions().dtype(torch::kLong).device(torch::kCPU);

    tensor_states = torch::zeros({batchSize, MAX_ABSTRACT_OBSERVATIONS}, options);
    tensor_next_states = torch::zeros({batchSize, MAX_ABSTRACT_OBSERVATIONS}, options);

    tensor_states = torch::empty({batchSize, MAX_ABSTRACT_OBSERVATIONS}, options);

    int i = 0;
    for(int random_index: random_indices) {

        copy(&obstaclesFOVcurrent[random_index][0][0], &obstaclesFOVcurrent[random_index][0][0] + FOV_WIDTH * FOV_WIDTH, &obstaclesFOV_current_temp[i][0][0]);
        copy(&enemiesFOVcurrent[random_index][0][0], &enemiesFOVcurrent[random_index][0][0] + FOV_WIDTH * FOV_WIDTH, &enemiesFOV_current_temp[i][0][0]);
        copy(&pathFOVcurrent[random_index][0][0], &pathFOVcurrent[random_index][0][0] + FOV_WIDTH * FOV_WIDTH, &pathFOV_current_temp[i][0][0]);

        copy(&obstaclesFOVnext[random_index][0][0], &obstaclesFOVnext[random_index][0][0] + FOV_WIDTH * FOV_WIDTH, &obstaclesFOV_next_temp[i][0][0]);
        copy(&enemiesFOVnext[random_index][0][0], &enemiesFOVnext[random_index][0][0] + FOV_WIDTH * FOV_WIDTH, &enemiesFOV_next_temp[i][0][0]);
        copy(&pathFOVnext[random_index][0][0], &pathFOVnext[random_index][0][0] + FOV_WIDTH * FOV_WIDTH, &pathFOV_next_temp[i][0][0]);

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

    auto tensor_obstacles_current = torch::from_blob(obstaclesFOV_current_temp, {batchSize, FOV_WIDTH, FOV_WIDTH}, options).unsqueeze(1).clone();
    auto tensor_enemies_current = torch::from_blob(enemiesFOV_current_temp, {batchSize, FOV_WIDTH, FOV_WIDTH}, options).unsqueeze(1).clone();
    auto tensor_path_current = torch::from_blob(pathFOV_current_temp, {batchSize, FOV_WIDTH, FOV_WIDTH}, options).unsqueeze(1).clone();
    // dimensions: Batch X Channels X FOV_WIDTH X FOV_WIDTH
    tensor_fov_channels_current = torch::cat({tensor_path_current, tensor_obstacles_current, tensor_enemies_current}, 1).clone();

    auto tensor_obstacles_next = torch::from_blob(obstaclesFOV_next_temp, {batchSize, FOV_WIDTH, FOV_WIDTH}, options).unsqueeze(1).clone();
    auto tensor_enemies_next = torch::from_blob(enemiesFOV_next_temp, {batchSize, FOV_WIDTH, FOV_WIDTH}, options).unsqueeze(1).clone();
    auto tensor_path_next = torch::from_blob(pathFOV_next_temp, {batchSize, FOV_WIDTH, FOV_WIDTH}, options).unsqueeze(1).clone();
    // dimensions: Batch X Channels X FOV_WIDTH X FOV_WIDTH
    tensor_fov_channels_next = torch::cat({tensor_path_next, tensor_obstacles_next, tensor_enemies_next}, 1).clone();

}

void ReplayMemory::storeExperience(observation &current, observation &next, int action, float reward, bool done) {
    if((not isBufferFull) and (idx + 1) == MAX_CAPACITY_REPLAY_BUFFER) {
        isBufferFull = true;
    }
    logger->logDebug("ReplayMemory::storeExperience")->endLineDebug();
#ifdef ENABLE_STATE_VECTOR_DUMP
    logStateVector(current);
#endif
    /**
     * Populate FOV for CNN
     */
     //TODO: Current must be taken before observation actually changes when the environment is dynamic
    cnn.populateFOVChannels(current.playerX, current.playerY, current.direction, obstaclesFOVcurrent[idx], enemiesFOVcurrent[idx], pathFOVcurrent[idx]);
    cnn.populateFOVChannels(next.playerX, next.playerY, next.direction, obstaclesFOVnext[idx], enemiesFOVnext[idx], pathFOVnext[idx]);

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

