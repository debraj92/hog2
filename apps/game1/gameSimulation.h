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

    const LOG_LEVEL LogLevel = LOG_LEVEL::INFO;

    std::unique_ptr<Logger> logger;

    int movePlayer(vector<vector<int>> &grid, std::vector<enemy>& enemies, observation &currentObservation, int* error);
    void moveEnemies(std::vector<enemy>& enemies);
    void fight(std::vector<enemy>& enemies);
    float calculateReward(std::vector<enemy>& enemies, const observation &ob, int action_error);
    void populateEnemies(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies);

    observation createObservationAfterAction(vector<vector<int>> &grid, std::vector<enemy>& enemies, observation ob, int action);

    bool isEpisodeComplete();

    void headStraightToDestination(vector<vector<int>> &grid, std::vector<enemy>& enemies, bool isInference);

    bool isMDPDone(observation &nextObservation);

public:

    explicit gameSimulation(vector<vector<int>> &grid) : coordinatesUtil(grid) {
        logger = std::make_unique<Logger>(LogLevel);
    }

    player* player1;

    void learnToPlay(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies);

    void play(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies);

    float getTotalRewardsCollected();

    void removeCharacters(std::vector<std::vector<int>> &grid);

    bool isDestinationReached();

};


#endif //EXAMPLE_GAMESIMULATION_H
