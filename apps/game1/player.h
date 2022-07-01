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
#include <testing.h>
#include "FOV_CNN/CNN_controller.h"

/// Testing
#ifdef TESTING
#include <gtest/gtest.h>
#endif

///// Change header-folder to load different RL models
//#include "DQN/dueling-DQN/RLNN_Agent.h"
//#include "DQN/DDQN/RLNN_Agent.h"
#include "DQN/Vanilla-DQN/RLNN_Agent.h"
//#include "DQN/dueling-DQN-bounded/RLNN_Agent.h"

using namespace RTS;

class player : public RLNN_Agent {

    const LOG_LEVEL LogLevel = LOG_LEVEL::INFO;

    const std::string DQN_MODEL_PATH = "/Users/debrajray/MyComputer/RL-A-STAR-THESIS/model";

    shared_ptr<findPath> fp;

    int episodeCount;

    int dqnTargetUpdateNextEpisode = MAX_EPISODES / 8;

    std::unique_ptr<Logger> logger;

    vector<std::vector<int>> grid;
    CNN_controller cnnController;

    void createEmptyGrid(vector<std::vector<int>> &grid);

public:

    int current_x;
    int current_y;
    int source_x;
    int source_y;
    int destination_x;
    int destination_y;
    int life_left;
    bool isExploring = false;

    bool playerDiedInPreviousEpisode = false;
    int resumeCount = 0;
    int restoreCellX;
    int restoreCellY;

    float total_rewards = 0;

    player(bool isTrainingMode) : cnnController(grid), RLNN_Agent(cnnController) {
        createEmptyGrid(grid);
        RLNN_Agent::setTrainingMode(isTrainingMode);
        if(not isTrainingMode) {
            RLNN_Agent::loadModel(DQN_MODEL_PATH);
        }
        logger = std::make_unique<Logger>(LogLevel);
    }

    void loadExistingModel();

    void initialize(int src_x, int src_y, int dest_x, int dest_y);

    void takeDamage(int points);

    void learnGame();

    void playGame(std::vector<std::vector<int>> &gridSource, std::vector<enemy> &enemies, int src_x, int src_y, int dest_x, int dest_y, TestResult &result);

    void observe(observation &ob, std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies);

    bool findPathToDestination(std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies, int src_x, int src_y, int dst_x, int dst_y);

    int selectAction(observation& currentState);

    void memorizeExperienceForReplay(observation &current, observation &next, int action, float reward, bool done);

    double learnWithDQN();

    bool recordRestoreLocation(std::vector<enemy> &enemies);

    void plotRewards(vector<double> &rewards);

    bool isResuming();

    void copyGrid(std::vector<std::vector<int>> &gridSource);

    /// Testing
#ifdef TESTING
    ReplayMemory* getAccessToReplayMemory() {
        return RLNN_Agent::getAccessToReplayMemory();
    }

    friend class Simulation_test;
    FRIEND_TEST(Simulation_test, test1);
    FRIEND_TEST(Simulation_test, test2);
    FRIEND_TEST(Simulation_test, test3);

#endif


};


#endif //EXAMPLE_PLAYER_H
