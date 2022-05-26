//
// Created by Debraj Ray on 2021-12-30.
//

#ifndef EXAMPLE_PLAYER_H
#define EXAMPLE_PLAYER_H

#include "state.h"
#include "vector"
#include <set>
#include "rl_control.h"
#include <cmath>
#include "findPath.h"

class player : public rl_control {

    shared_ptr<findPath> fp;

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
    bool ontrack;

    player() {
    }

    void initialize(int src_x, int src_y, int dest_x, int dest_y);

    void takeDamage(int points);

    void learnGame(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies);

    void playGame(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies, int src_x, int src_y, int dest_x, int dest_y);

    void observe(observation &ob, std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies, bool isRedirect);

    int getDirection();

    void findPathToDestination(std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies, int src_x, int src_y, int dst_x, int dst_y);

    void follow();

    bool isOnTrack();

    void evaluateActionQValues(int reward, observation& next_observation, int current_action);

    void getNextStateForInference(observation& next_observation);

    void printBoard(std::vector<std::vector<int>> &grid);

    void selectRandomSourceAndDestinationCoordinates(std::mt19937 &rng, std::uniform_int_distribution<std::mt19937::result_type> &randGen, std::vector<std::vector<int>> &grid, int &src_x, int &src_y, int &dest_x, int &dest_y);
};


#endif //EXAMPLE_PLAYER_H
