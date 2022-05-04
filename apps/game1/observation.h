//
// Created by Debraj Ray on 2022-01-03.
//

#ifndef EXAMPLE_OBSERVATION_H
#define EXAMPLE_OBSERVATION_H

#include "gameConstants.h"
#include "observation.h"
#include "findPath.h"
#include <iostream>
#include "enemy/enemy.h"

class observation {

public:
    int direction = 0;
    double enemy_distance = 5*VISION_RADIUS;
    double enemy_cosine = -1; // cos theta
    int trajectory = 0;

    int obstacle_front = VISION_RADIUS + 1; // can be at distance 0/1/2 deviation
    int obstacle_left = VISION_RADIUS + 1;
    int obstacle_right = VISION_RADIUS + 1;
    int obstacle_front_left = VISION_RADIUS + 1;
    int obstacle_front_right = VISION_RADIUS + 1;

    void locateTrajectoryAndDirection(const shared_ptr<findPath>& fp, int current_x, int current_y);
    void updateObstacleDistances(std::vector<std::vector<int>> &grid, int x, int y);
    void locateEnemies(std::vector<enemy>& enemies, int current_x, int current_y);

    void printData();
};

#endif //EXAMPLE_OBSERVATION_H
