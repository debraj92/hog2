//
// Created by Debraj Ray on 2022-06-21.
//

#include <gtest/gtest.h>
#include "../gameConstants.h"
#include "../fixedobstacles.h"
#include "../gameSimulation.h"
#include "../player.h"
#include "../Logger.h"
#include "../enemy/enemy.h"

using namespace std;

class Simulation_test : public ::testing::Test {

};

void createAllFixedObstacles(vector<std::vector<int>> &grid, int TOTAL_FIXED_OBSTACLES, int blockObstacles[][4]) {

    // fill with static obstacles
    FixedObstacles fixedObstacles;
    for(int obstacle=0; obstacle<TOTAL_FIXED_OBSTACLES; obstacle++) {
        int x_s = blockObstacles[obstacle][0];
        int x_e = blockObstacles[obstacle][1];
        int y_s = blockObstacles[obstacle][2];
        int y_e = blockObstacles[obstacle][3];
        fixedObstacles.createBlockObstacle(x_s, x_e, y_s, y_e, grid);
    }
}

TEST_F(Simulation_test, test1) {
    /**
     * Set configs:
     *
     * MAX_EPISODES = 1;
     * SESSION_TIMEOUT = 10;
     * GRID_SPAN = 7
     * SWITCH_TO_EXPLOITATION_ONLY_PERCENT = 1000;
     */
    Logger::GLOBAL_LOG_LEVEL = LOG_LEVEL::DEBUG;

    player pl1(true);
    vector<std::vector<int>> grid;
    pl1.createEmptyGrid(grid);
    gameSimulation game(grid);
    game.player1 = &pl1;

    vector<enemy> enemies;
    enemy f1(2, 2, 1, true);
    enemies.push_back(f1);

    pl1.initialize(0, 0, 6, 6);
    pl1.epoch = 1;
    game.learnToPlay(grid, enemies);

    auto replayBuf_ptr = pl1.getAccessToReplayMemory();
    assert (replayBuf_ptr->rewards[0] == REWARD_TRACK_ONE_DIV);
    assert (not replayBuf_ptr->dones[0]);
    assert (replayBuf_ptr->buffer_actions[0] == ACTION_DODGE_DIAGONAL_RIGHT);

    assert (replayBuf_ptr->rewards[1] == REWARD_ACTION_LR);
    assert (not replayBuf_ptr->dones[1]);
    assert (replayBuf_ptr->buffer_actions[1] == ACTION_DODGE_LEFT);

    assert (replayBuf_ptr->rewards[2] == REWARD_ACTION_LR);
    assert (not replayBuf_ptr->dones[2]);
    assert (replayBuf_ptr->buffer_actions[2] == ACTION_DODGE_RIGHT);

    assert (replayBuf_ptr->idx == 6);
    cout<<pl1.restoreCellX<<", "<<pl1.restoreCellY<<endl;
    assert(pl1.restoreCellX == 3 and pl1.restoreCellY == 3);


}

#ifdef BUGFIXED

/// The outputs are different because Left and Right actions were added so the actions selected by the random
/// seed are different

TEST_F(Simulation_test, test2) {
    /**
     * Set configs:
     *
     * MAX_EPISODES = 1;
     * SESSION_TIMEOUT = 5;
     * GRID_SPAN = 7
     * SWITCH_TO_EXPLOITATION_ONLY_PERCENT = 1000;
     */
    Logger::GLOBAL_LOG_LEVEL = LOG_LEVEL::DEBUG;

    player pl1(true);
    vector<std::vector<int>> grid;
    pl1.createEmptyGrid(grid);

    const int TOTAL_FIXED_OBSTACLES = 1;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {2, 4, 2, 3}

    };
    createAllFixedObstacles(grid, TOTAL_FIXED_OBSTACLES, blockObstacles);


    gameSimulation game(grid);
    game.player1 = &pl1;

    vector<enemy> enemies;
    fixedEnemy f1(3, 1, 1);
    fixedEnemy f2(5, 4, 2);
    enemies.push_back(f1);
    enemies.push_back(f2);

    pl1.initialize(0, 0, 6, 6);
    pl1.episodeCount = 1;
    pl1.seedAction = 10;
    pl1.seedExplore = 20;
    game.learnToPlay(grid, enemies);

    auto replayBuf_ptr = pl1.getAccessToReplayMemory();
    assert (replayBuf_ptr->rewards[0] == -1);
    assert (not replayBuf_ptr->dones[0]);
    assert (replayBuf_ptr->buffer_actions[0] == ACTION_DODGE_DIAGONAL_RIGHT);

    assert (replayBuf_ptr->rewards[1] == -1);
    assert (not replayBuf_ptr->dones[1]);
    assert (replayBuf_ptr->buffer_actions[1] == ACTION_DODGE_DIAGONAL_LEFT);

    assert (replayBuf_ptr->rewards[2] == -1);
    assert (not replayBuf_ptr->dones[2]);
    assert (replayBuf_ptr->buffer_actions[2] == ACTION_DODGE_DIAGONAL_LEFT);

    assert (replayBuf_ptr->rewards[3] == -1);
    assert (not replayBuf_ptr->dones[3]);
    assert (replayBuf_ptr->buffer_actions[3] == ACTION_STRAIGHT);

    assert (replayBuf_ptr->rewards[4] == 20);
    assert (replayBuf_ptr->dones[4]);
    assert (replayBuf_ptr->buffer_actions[4] == ACTION_DODGE_DIAGONAL_RIGHT);

    assert (replayBuf_ptr->idx == 5);

    assert(not pl1.playerDiedInPreviousEpisode);

}

TEST_F(Simulation_test, test3) {
    /**
     * Set configs:
     *
     * MAX_EPISODES = 1;
     * SESSION_TIMEOUT = 5;
     * GRID_SPAN = 7
     * SWITCH_TO_EXPLOITATION_ONLY_PERCENT = 1000;
     */
    Logger::GLOBAL_LOG_LEVEL = LOG_LEVEL::DEBUG;

    player pl1(true);
    vector<std::vector<int>> grid;
    pl1.createEmptyGrid(grid);

    const int TOTAL_FIXED_OBSTACLES = 1;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {2, 4, 2, 3}

    };
    createAllFixedObstacles(grid, TOTAL_FIXED_OBSTACLES, blockObstacles);


    gameSimulation game(grid);
    game.player1 = &pl1;

    vector<enemy> enemies;
    fixedEnemy f1(3, 1, 1);
    fixedEnemy f2(5, 4, 2);
    enemies.push_back(f1);
    enemies.push_back(f2);

    pl1.initialize(GRID_SPAN/2, GRID_SPAN -1, GRID_SPAN - 1, 0);
    pl1.episodeCount = 1;
    pl1.seedAction = 5;
    pl1.seedExplore = 8;
    game.learnToPlay(grid, enemies);

    auto replayBuf_ptr = pl1.getAccessToReplayMemory();
    assert (replayBuf_ptr->rewards[0] == 20);
    assert (replayBuf_ptr->dones[0]);
    assert (replayBuf_ptr->buffer_actions[0] == ACTION_STRAIGHT);

    assert (replayBuf_ptr->rewards[1] == -1);
    assert (not replayBuf_ptr->dones[1]);
    assert (replayBuf_ptr->buffer_actions[1] == ACTION_DODGE_DIAGONAL_RIGHT);

    assert (replayBuf_ptr->rewards[2] == -20);
    assert (replayBuf_ptr->dones[2]);
    assert (replayBuf_ptr->buffer_actions[2] == ACTION_DODGE_DIAGONAL_LEFT);

    assert (replayBuf_ptr->idx == 3);

    assert(pl1.playerDiedInPreviousEpisode);
    assert(pl1.restoreCellX == 6 and pl1.restoreCellY == 3);

}
#endif