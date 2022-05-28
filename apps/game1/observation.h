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
#include "vector"

class observation {

    struct enemy_distance_cosine {
        double distance;
        double cosine;
    };

    void setDirectionAngles(int (&angles)[9]);
    void updateEnemyDistanceAndAngles(vector<enemy_distance_cosine>& enemy_distance_cosines);

public:
    int direction = 0;
    /**
     * Assumption: Maximum 4 enemies within the vision radius
     */
    double enemy_distance_1 = 5*VISION_RADIUS;
    double enemy_cosine_1 = -1; // cos theta

    double enemy_distance_2 = 5*VISION_RADIUS;
    double enemy_cosine_2 = -1; // cos theta

    double enemy_distance_3 = 5*VISION_RADIUS;
    double enemy_cosine_3 = -1; // cos theta

    double enemy_distance_4 = 5*VISION_RADIUS;
    double enemy_cosine_4 = -1; // cos theta

    vector<enemy> enemies_in_vision;

    int rerouteDistance = 1000;

    int trajectory = 0;
    double destination_distance = 1000;
    double destination_cosine = -1;

    int obstacle_front = VISION_RADIUS + 1; // can be at distance 0/1/2 deviation
    int obstacle_left = VISION_RADIUS + 1;
    int obstacle_right = VISION_RADIUS + 1;
    int obstacle_front_left = VISION_RADIUS + 1;
    int obstacle_front_right = VISION_RADIUS + 1;

    void locateTrajectoryAndDirection(const shared_ptr<findPath>& fp, int current_x, int current_y, int destination_x, int destination_y);
    void updateObstacleDistances(std::vector<std::vector<int>> &grid, int x, int y);
    void locateEnemies(std::vector<enemy>& enemies, int current_x, int current_y);
    void locateDestination(int current_x, int current_y, int destination_x, int destination_y);

    void printData();

    void redirect(int current_x, int current_y, int destination_x, int destination_y);
    void resetRerouteDistance();
};

#endif //EXAMPLE_OBSERVATION_H
