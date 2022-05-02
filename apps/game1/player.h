//
// Created by Debraj Ray on 2021-12-30.
//

#ifndef EXAMPLE_PLAYER_H
#define EXAMPLE_PLAYER_H

#include "state.h"
#include "vector"
#include <set>
#include "enemy/enemy.h"
#include "rl_control.h"
#include <cmath>
#include "findPath.h"

class player : public rl_control {

    unique_ptr<findPath> fp;

    //state cur_state;
    void locateTrajectoryAndDirection(observation& ob);
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

    observation createObservation(std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies);

    int getDirection();

    void findPathToDestination(std::vector<std::vector<int>> &grid, int src_x, int src_y, int dst_x, int dst_y);

    void follow();

    bool isOnTrack();

    void evaluateActionQValues(int reward, observation& next_observation, int current_action, std::vector<std::vector<int>> &grid);

    void printBoard(std::vector<std::vector<int>> &grid);
};


#endif //EXAMPLE_PLAYER_H
