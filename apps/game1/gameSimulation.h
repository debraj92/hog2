//
// Created by Debraj Ray on 2021-12-30.
//

#ifndef EXAMPLE_GAMESIMULATION_H
#define EXAMPLE_GAMESIMULATION_H

#include <iostream>
#include "vector"
#include "enemy/enemy.h"
#include "gameConstants.h"
#include "player.h"
#include "findPath.h"
#include "coordinatesUtil.h"

using namespace std;

class gameSimulation : public coordinatesUtil {

    int total_rewards = 0;

    int movePlayer(int* error, bool isInference = false);
    void moveEnemies(std::vector<enemy>& enemies);
    void fight(std::vector<enemy>& enemies);
    int calculateReward(std::vector<enemy>& enemies, observation &ob, int action_error);
    void populateEnemies(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies);

    void printAction(int action);

    bool isDestinationReached();

public:

    explicit gameSimulation(vector<vector<int>> &grid) : coordinatesUtil(grid) {

    }

    player* player1;

    void learnToPlay(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies);

    void play(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies);

    int getTotalRewardsCollected();

    void reset(std::vector<std::vector<int>> &grid);

};


#endif //EXAMPLE_GAMESIMULATION_H
