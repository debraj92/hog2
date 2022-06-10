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

    struct enemy_attributes {
        float distance;
        float angle;
        float risk_measure;
    };

    void setDirectionAngles(int (&angles)[9]);
    void updateEnemyDistanceAndAngles(vector<enemy_attributes>& enemy_properties);

public:
    int playerX = 0;
    int playerY = 0;
    float playerLifeLeft = 0;

    int direction = 0; // one hot (8 values + 1 unknown = total 9 combinations)

    /**
     * Assumption: Maximum 4 enemies within the vision radius
     */
    float enemy_distance_1 = MAX_DISTACE;
    float enemy_angle_1 = -1;
    float enemy_risk_1 = 0;

    float enemy_distance_2 = MAX_DISTACE;
    float enemy_angle_2 = -1;
    float enemy_risk_2 = 0;

    float enemy_distance_3 = MAX_DISTACE;
    float enemy_angle_3 = -1;
    float enemy_risk_3 = 0;

    float enemy_distance_4 = MAX_DISTACE;
    float enemy_angle_4 = -1;
    float enemy_risk_4 = 0;

    int rerouteDistance = MAX_DISTACE;

    int trajectory = 0; // one hot (8 values, 4 one deviation another 4 two deviations)

    double destination_distance = 1000;
    double destination_cosine = -1;

    int obstacle_front = 1000; // default at infinity
    int obstacle_left = 1000;
    int obstacle_right = 1000;
    int obstacle_front_left = 1000;
    int obstacle_front_right = 1000;

    void locateTrajectoryAndDirection(const shared_ptr<findPath>& fp, int destination_x, int destination_y);
    void updateObstacleDistances(std::vector<std::vector<int>> &grid);
    void locateEnemies(std::vector<enemy>& enemies);

    void printData();

    void redirect(int current_x, int current_y, int destination_x, int destination_y);
    void resetRerouteDistance();

    void flattenObservationToVector(float (&observation_vector)[MAX_ABSTRACT_OBSERVATIONS]);
};

#endif //EXAMPLE_OBSERVATION_H
