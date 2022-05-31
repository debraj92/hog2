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
        double distance;
        double angle;
        double risk_measure;
    };

    void setDirectionAngles(int (&angles)[9]);
    void updateEnemyDistanceAndAngles(vector<enemy_attributes>& enemy_properties);

public:
    int direction = 0; // one hot (8 values + 1 unknown = total 9 combinations)

    /**
     * Assumption: Maximum 4 enemies within the vision radius
     */
    double enemy_distance_1 = 5*VISION_RADIUS;
    double enemy_angle_1 = -1;
    double enemy_risk_1 = 0;

    double enemy_distance_2 = 5*VISION_RADIUS;
    double enemy_angle_2 = -1;
    double enemy_risk_2 = 0;

    double enemy_distance_3 = 5*VISION_RADIUS;
    double enemy_angle_3 = -1;
    double enemy_risk_3 = 0;

    double enemy_distance_4 = 5*VISION_RADIUS;
    double enemy_angle_4 = -1;
    double enemy_risk_4 = 0;

    int rerouteDistance = 1000;

    int trajectory = 0; // one hot (8 values, 4 one deviation another 4 two deviations)

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

    void printData();

    void redirect(int current_x, int current_y, int destination_x, int destination_y);
    void resetRerouteDistance();

    void flattenObservationToVector(double (&observation_vector)[MAX_ABSTRACT_OBSERVATIONS]);
};

#endif //EXAMPLE_OBSERVATION_H
