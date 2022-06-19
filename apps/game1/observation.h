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
#include "Logger.h"

class observation {

    LOG_LEVEL LogLevel = LOG_LEVEL::INFO;

    std::shared_ptr<Logger> logger;

    struct enemy_attributes {
        float distance;
        float angle;
        float risk_measure;
        int id;
    };

    void setDirectionAngles(int (&angles)[9]);
    void updateEnemyDistanceAndAngles(vector<enemy_attributes>& enemy_properties);

public:

    observation() {
        logger = std::make_shared<Logger>(LogLevel);
    }

    int playerX = 0;
    int playerY = 0;
    float playerLifeLeft = 0;

    int direction = 0; // one hot (8 values + 1 unknown = total 9 combinations)

    /**
     * Assumption: Maximum 4 enemies within the vision radius
     */
    float enemy_distance_1 = MAX_DISTANCE;
    float enemy_angle_1 = 0;
    float enemy_risk_1 = 0;
    int enemy_id_1 = -1;

    float enemy_distance_2 = MAX_DISTANCE;
    float enemy_angle_2 = 0;
    float enemy_risk_2 = 0;
    int enemy_id_2 = -1;

    float enemy_distance_3 = MAX_DISTANCE;
    float enemy_angle_3 = 0;
    float enemy_risk_3 = 0;
    int enemy_id_3 = -1;

    // For backward compatibility with table based rl
    float enemy_distance_4 = MAX_DISTANCE;
    float enemy_angle_4 = 0;
    float enemy_risk_4 = 0;
    int enemy_id_4 = -1;

    int rerouteDistance = MAX_DISTANCE;

    int trajectory = 0; // one hot (8 values, 4 one deviation another 4 two deviations)
    int trajectory_left = 0;
    int trajectory_right = 0;
    int trajectory_front = 0;
    int trajectory_on_track = 0;
    int trajectory_off_track = 0;

    double destination_distance = 1000;
    double destination_cosine = -1;

    int obstacle_front = MAX_DISTANCE; // default at infinity
    int obstacle_left = MAX_DISTANCE;
    int obstacle_right = MAX_DISTANCE;
    int obstacle_front_left = MAX_DISTANCE;
    int obstacle_front_right = MAX_DISTANCE;

    void locateTrajectoryAndDirection(const shared_ptr<findPath>& fp, int destination_x, int destination_y);
    void locateRelativeTrajectory();
    void updateObstacleDistances(std::vector<std::vector<int>> &grid);
    void locateEnemies(std::vector<enemy>& enemies);

    void printData();

    void redirect(int current_x, int current_y, int destination_x, int destination_y);
    void resetRerouteDistance();

    void flattenObservationToVector(float (&observation_vector)[MAX_ABSTRACT_OBSERVATIONS]);
};

#endif //EXAMPLE_OBSERVATION_H
