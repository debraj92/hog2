//
// Created by Debraj Ray on 2021-12-30.
//

#ifndef EXAMPLE_PLAYER_H
#define EXAMPLE_PLAYER_H

#include "state.h"
#include "vector"
#include <set>
#include <cmath>
#include "findPath.h"
#include "TestResult.h"
#include "Logger.h"

///// Change header-folder to load different RL models
//#include "DQN/dueling-DQN/RLNN_Agent.h"
//#include "DQN/DDQN/RLNN_Agent.h"
//#include "DQN/Vanilla-DQN/RLNN_Agent.h"
#include "DQN/dueling-DQN-bounded/RLNN_Agent.h"

using namespace RTS;

class player : public RLNN_Agent {

    const LOG_LEVEL LogLevel = LOG_LEVEL::INFO;

    shared_ptr<findPath> fp;
    shared_ptr<findPath> fp_temp_reroute;

    int episodeCount;

    int dqnTargetUpdateNextEpisode = MAX_EPISODES / 8;

    std::unique_ptr<Logger> logger;

    //state cur_state;
    void reset(std::vector<std::vector<int>> &grid);

public:

    int current_x;
    int current_y;
    int source_x;
    int source_y;
    int destination_x;
    int destination_y;
    int life_left;
    bool isExploring = false;

    float total_rewards = 0;

    int train_step = 0;

    player(bool isTrainingMode) {
        RLNN_Agent::setTrainingMode(isTrainingMode);
        logger = std::make_unique<Logger>(LogLevel);
    }

    void initialize(int src_x, int src_y, int dest_x, int dest_y);

    void takeDamage(int points);

    void learnGame(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies);

    void playGame(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies, int src_x, int src_y, int dest_x, int dest_y, TestResult &result);

    void observe(observation &ob, std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies, bool isRedirect);

    int getDirection();

    void findPathToDestination(std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies, int src_x, int src_y, int dst_x, int dst_y);

    void follow();

    int switchToNewRoute(observation &ob);

    bool isOnTrack();

    void findNewRoute(std::vector<std::vector<int>> &grid, observation &ob, std::vector<enemy>& enemies, int src_x, int src_y, int dst_x, int dst_y);

    int selectAction(observation& currentState);

    void memorizeExperienceForReplay(observation &current, observation &next, int action, float reward, bool done);

    double learnWithDQN();
};


#endif //EXAMPLE_PLAYER_H
