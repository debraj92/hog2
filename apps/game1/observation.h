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
#include "FOV_CNN/CNN_controller.h"

class observation {

    LOG_LEVEL LogLevel = LOG_LEVEL::INFO;

    std::shared_ptr<Logger> logger;

    struct enemy_attributes {
        float distance;
        float angle;
        float risk_measure;
        int id;
        bool isFixed;
        int moves_left;
        int enemyX;
        int enemyY;
    };

    void updateEnemyDistanceAndAngles(vector<enemy_attributes>& enemy_properties);

public:

    observation() {
        logger = std::make_shared<Logger>(LogLevel);
    }

    int playerX = 0;
    int playerY = 0;

    int destinationX = 0;
    int destinationY = 0;

    float playerLifeLeft = 0;

    int direction = 0; // one hot (8 values + 1 unknown = total 9 combinations)

    bool isGoalInSight = false;
    int actionInPreviousState;

    /**
     * Assumption: Maximum 6 enemies within the vision radius
     */
    float enemy_distance_1 = MAX_DISTANCE;
    float enemy_angle_1 = 0;
    float enemy_risk_1 = 0;
    int enemy_id_1 = -1;
    int enemy_is_fixed_1 = 1;
    int enemy_moves_left_1 = 0;

    float enemy_distance_2 = MAX_DISTANCE;
    float enemy_angle_2 = 0;
    float enemy_risk_2 = 0;
    int enemy_id_2 = -1;
    int enemy_is_fixed_2 = 1;
    int enemy_moves_left_2 = 0;

    float enemy_distance_3 = MAX_DISTANCE;
    float enemy_angle_3 = 0;
    float enemy_risk_3 = 0;
    int enemy_id_3 = -1;
    int enemy_is_fixed_3 = 1;
    int enemy_moves_left_3 = 0;

    float enemy_distance_4 = MAX_DISTANCE;
    float enemy_angle_4 = 0;
    float enemy_risk_4 = 0;
    int enemy_id_4 = -1;
    int enemy_is_fixed_4 = 1;
    int enemy_moves_left_4 = 0;

    float action_straight_atRisk = 0;
    float action_frontLeft_atRisk = 0;
    float action_frontRight_atRisk = 0;
    float action_left_atRisk = 0;
    float action_right_atRisk = 0;

    bool isPlayerInHotPursuit = false;

    int rerouteDistance = MAX_DISTANCE;

    int trajectory = 0; // one hot (8 values, 4 one deviation another 4 two deviations)
    int trajectory_left = 0;
    int trajectory_right = 0;
    int trajectory_front = 0;
    int trajectory_on_track = 0;
    int trajectory_off_track = 0;

    int countNodeNumbersInDirection = 0;

    double destination_distance = 1000;
    double destination_cosine = -1;

    int obstacle_front = MAX_DISTANCE; // default at infinity

    int obstacle_front_left = MAX_DISTANCE;
    int obstacle_front_right = MAX_DISTANCE;
    int obstacle_blind_left = MAX_DISTANCE;
    int obstacle_blind_right = MAX_DISTANCE;


    int obstacle_left = MAX_DISTANCE; // deprecated
    int obstacle_right = MAX_DISTANCE; // deprecated

    // FOV for CNN
    float obstaclesFOV[FOV_WIDTH][FOV_WIDTH];
    float enemiesFOV[FOV_WIDTH][FOV_WIDTH];
    float pathFOV[FOV_WIDTH][FOV_WIDTH];

    void locateTrajectoryAndDirection(const shared_ptr<findPath>& fp);
    void locateRelativeTrajectory();
    void updateObstacleDistances(std::vector<std::vector<int>> &grid);
    void locateEnemies(std::vector <std::vector<int>> &grid, std::vector<enemy>& enemies);

    void printData();

    void flattenObservationToVector (float (&observation_vector)[MAX_ABSTRACT_OBSERVATIONS]) const;

    void setGoalInSight(int probeX, int probeY);

    void findDestination(bool isTraining);

    void recordFOVForCNN(CNN_controller& cnn, const shared_ptr<findPath>& fp);

    void printRelativeTrajectory();

    void printEnemyDistanceAndAngles();

    void markRiskyActions(std::vector <std::vector<int>> &grid, vector<enemy_attributes>& enemy_properties);

    int getShortestDistanceBetweenPoints(int x1, int y1, int x2, int y2);

};

#endif //EXAMPLE_OBSERVATION_H
