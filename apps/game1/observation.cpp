//
// Created by Debraj Ray on 2022-01-08.
//

#include "observation.h"
#include "coordinatesUtil.h"
#include "objectLocator.h"
#include "algorithm"

#include <xtensor/xrandom.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xnpy.hpp>
#include <xtensor/xsort.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xio.hpp>

using namespace std;
using namespace xt;

void observation::updateObstacleDistances(std::vector <std::vector<int>> &grid) {
    int x = playerX;
    int y = playerY;
    coordinatesUtil coordinates(grid);

    // front
    int next_x = x;
    int next_y = y;

    for(int i=1; i<=VISION_RADIUS; i++) {
        if (coordinates.setStraightActionCoordinates(next_x, next_y, direction) == -1) {
            obstacle_front = i;
            break;
        }
    }

    // front-left and blind-left
    next_x = x;
    next_y = y;

    for(int i=1; i<=VISION_RADIUS; i++) {
        if (i==VISION_RADIUS) {
            int next_x_ = next_x;
            int next_y_ = next_y;
            if (coordinates.setStraightActionCoordinates(next_x_, next_y_, direction) == -1) {
                obstacle_blind_left = i;
            }
        }
        if (coordinates.setDodgeDiagonalLeftActionCoordinates(next_x, next_y, direction) == -1) {
            obstacle_front_left = i;
            if(i == 1) {
                obstacle_blind_left = i;
            }
            break;
        }
    }

    // front-right and blind-right
    next_x = x;
    next_y = y;

    for(int i=1; i<=VISION_RADIUS; i++) {
        if (i==VISION_RADIUS) {
            int next_x_ = next_x;
            int next_y_ = next_y;
            if (coordinates.setStraightActionCoordinates(next_x_, next_y_, direction) == -1) {
                obstacle_blind_right = i;
            }
        }
        if (coordinates.setDodgeDiagonalRightActionCoordinates(next_x, next_y, direction) == -1) {
            obstacle_front_right = i;
            if(i == 1) {
                obstacle_blind_right = i;
            }
            break;
        }
    }
}

void observation::printData() {

    logger->logDebug("Player Direction ")->logDebug(direction)->endLineDebug();
    logger->logDebug("Player trajectory ")->logDebug(trajectory)->endLineDebug();
    logger->logDebug("Obstacle front ")->logDebug(obstacle_front)->endLineDebug();
    logger->logDebug("Obstacle left ")->logDebug(obstacle_left)->endLineDebug();
    logger->logDebug("Obstacle front left ")->logDebug(obstacle_front_left)->endLineDebug();
    logger->logDebug("Obstacle right ")->logDebug(obstacle_right)->endLineDebug();
    logger->logDebug("Obstacle front right ")->logDebug(obstacle_front_right)->endLineDebug();
}

/**
 *   -->y
 *   |
 *   v
 *   x
 */
void observation::locateTrajectoryAndDirection(const shared_ptr<findPath>& fp) {
    logger->logDebug("locateTrajectoryAndDirection ")->endLineDebug();

    int current_x = playerX;
    int current_y = playerY;
    if (fp->isOnTrack(current_x, current_y)) {
        trajectory = on_track;
        direction = fp->pathDirection(current_x, current_y);
        countNodeNumbersInDirection = MAX_DISTANCE;
        logger->logDebug("Direction ")->logDebug(direction)->endLineDebug();
        logger->logDebug("Trajectory ")->logDebug(trajectory)->endLineDebug();
        return;
    }

    bool pathFound = false;
    int row, col;
    int matches = 0;
    int direction_continued = 0;
    int max_matches = 0;
    int max_direction_continued = 0;
    int temp_direction = 0;
    int temp_trajectory = 0;
    for (int i=1; i<=VISION_RADIUS; i++) {
        row = current_x - i;
        if (row >= 0) {
            xarray<int>::shape_type shape = {9};
            xarray<int> directionOccurrences = xarray<int>::from_shape(shape);
            directionOccurrences.fill(0);
            directionOccurrences[0] = -100;
            for(col=current_y - i; col<=current_y + i; col++) {
                if (col >= 0 && col <= GRID_SPAN - 1) {
                    setGoalInSight(row, col);
                    if (fp->isOnTrackNoMemorizing(row, col)) {
                        temp_direction = fp->pathDirection(row, col);
                        temp_trajectory = (i * 10) + 1;
                        directionOccurrences[temp_direction] += fp->getNodeOrder(row, col) + 1;
                        pathFound = true;
                        matches++;
                        if(col > current_y - i and fp->isOnTrackNoMemorizing(row, col - 1)
                           and temp_direction == fp->pathDirection(row, col - 1)) {
                            direction_continued++;
                        }
                    }
                }
            }
            if (matches > max_matches) {
                max_matches = matches;
                max_direction_continued = direction_continued;
                direction = argmax(directionOccurrences)[0];
                trajectory = temp_trajectory;
                countNodeNumbersInDirection = directionOccurrences[direction];
            }
            matches = 0;
            direction_continued = 0;
        }
        row = current_x + i;
        if (row <= GRID_SPAN - 1) {
            xarray<int>::shape_type shape = {9};
            xarray<int> directionOccurrences = xarray<int>::from_shape(shape);
            directionOccurrences.fill(0);
            directionOccurrences[0] = -100;
            for(col=current_y - i; col<=current_y + i; col++) {
                if (col >= 0 && col <= GRID_SPAN - 1) {
                    setGoalInSight(row, col);
                    if (fp->isOnTrackNoMemorizing(row, col)) {
                        temp_direction = fp->pathDirection(row, col);
                        temp_trajectory = (i * 10) + 2;
                        directionOccurrences[temp_direction] += fp->getNodeOrder(row, col) + 1;
                        matches++;
                        pathFound = true;
                        if(col > current_y - i and fp->isOnTrackNoMemorizing(row, col - 1)
                           and temp_direction == fp->pathDirection(row, col - 1)) {
                            direction_continued++;
                        }
                    }
                }
            }
            int max_direction = argmax(directionOccurrences)[0];
            if(matches > max_matches
            or (matches == max_matches and direction_continued > max_direction_continued)
            or (matches == max_matches and directionOccurrences [max_direction] > countNodeNumbersInDirection)) {
                max_matches = matches;
                max_direction_continued = direction_continued;
                direction = max_direction;
                trajectory = temp_trajectory;
                countNodeNumbersInDirection = directionOccurrences[direction];
            }
            matches = 0;
            direction_continued = 0;
        }
        col = current_y - i;
        if (col >= 0) {
            xarray<int>::shape_type shape = {9};
            xarray<int> directionOccurrences = xarray<int>::from_shape(shape);
            directionOccurrences.fill(0);
            directionOccurrences[0] = -100;
            for(row=current_x - i; row<=current_x + i; row++) {
                if (row >= 0 && row <= GRID_SPAN - 1) {
                    setGoalInSight(row, col);
                    if (fp->isOnTrackNoMemorizing(row, col)) {
                        temp_direction = fp->pathDirection(row, col);
                        temp_trajectory = (i * 10) + 3;
                        directionOccurrences[temp_direction] += fp->getNodeOrder(row, col) + 1;
                        matches++;
                        pathFound = true;
                        if(row > current_x - i and fp->isOnTrackNoMemorizing(row - 1, col)
                           and temp_direction == fp->pathDirection(row - 1, col)) {
                            direction_continued++;
                        }
                    }
                }
            }
            int max_direction = argmax(directionOccurrences)[0];
            if(matches > max_matches or
            (matches == max_matches and direction_continued > max_direction_continued)
            or (matches == max_matches and directionOccurrences [max_direction] > countNodeNumbersInDirection)) {
                max_matches = matches;
                max_direction_continued = direction_continued;
                direction = max_direction;
                trajectory = temp_trajectory;
                countNodeNumbersInDirection = directionOccurrences[direction];
            }
            matches = 0;
            direction_continued = 0;
        }
        col = current_y + i;
        if (col <= GRID_SPAN - 1) {
            xarray<int>::shape_type shape = {9};
            xarray<int> directionOccurrences = xarray<int>::from_shape(shape);
            directionOccurrences.fill(0);
            directionOccurrences[0] = -100;
            for(row=current_x - i; row<=current_x + i; row++) {
                if (row >= 0 && row <= GRID_SPAN - 1) {
                    setGoalInSight(row, col);
                    if (fp->isOnTrackNoMemorizing(row, col)) {
                        temp_direction = fp->pathDirection(row, col);
                        temp_trajectory = (i * 10) + 4;
                        directionOccurrences[temp_direction] += fp->getNodeOrder(row, col) + 1;
                        matches++;
                        pathFound = true;
                        if(row > current_x - i and fp->isOnTrackNoMemorizing(row - 1, col)
                           and temp_direction == fp->pathDirection(row - 1, col)) {
                            direction_continued++;
                        }
                    }
                }
            }
            int max_direction = argmax(directionOccurrences)[0];
            if(matches > max_matches
            or (matches == max_matches and direction_continued > max_direction_continued)
            or (matches == max_matches and directionOccurrences [max_direction] > countNodeNumbersInDirection)) {
                max_matches = matches;
                max_direction_continued = direction_continued;
                direction = max_direction;
                trajectory = temp_trajectory;
                countNodeNumbersInDirection = directionOccurrences[direction];
            }
            matches = 0;
            direction_continued = 0;
        }
        if(pathFound) {
            logger->logDebug("Direction ")->logDebug(direction)->endLineDebug();
            logger->logDebug("Trajectory ")->logDebug(trajectory)->endLineDebug();
            return;
        }
    }
    trajectory = 0;
    direction = 0;
    countNodeNumbersInDirection = 0;
}

void observation::locateEnemies(std::vector<enemy> &enemies) {
    objectLocator ol;
    vector<enemy_attributes> enemy_properties;
    for(const enemy& e: enemies) {
        ol.locateObject(playerX, playerY, direction, e.current_x, e.current_y);
        double distance = ol.getObjectDistance();
        if (distance <= VISION_RADIUS) {
            ol.measureRiskAndObjectAngle();
            enemy_properties.push_back({ol.getObjectDistance(), ol.getObjectAngle(), ol.getObjectRiskFeature(), e.id});
        }
    }

    logger->logDebug("Number of enemies in vision ")->logDebug(enemy_properties.size())->endLineDebug();

    sort(enemy_properties.begin(), enemy_properties.end(),
         [](const enemy_attributes &e1, const enemy_attributes &e2) -> bool
         {
             return e1.risk_measure > e2.risk_measure;
         });

    updateEnemyDistanceAndAngles(enemy_properties);

}

void observation::updateEnemyDistanceAndAngles(vector<enemy_attributes>& enemy_properties) {
    // Ignoring enemy count above 3

    if(enemy_properties.size() >= 3) {
        enemy_distance_3 = enemy_properties[2].distance;
        enemy_angle_3 = enemy_properties[2].angle;
        enemy_risk_3 = enemy_properties[2].risk_measure;
        enemy_id_3 = enemy_properties[2].id;
        enemy_properties.pop_back();
    } else {
        enemy_distance_3 = MAX_DISTANCE;
        enemy_angle_3 = 0;
        enemy_risk_3 = 0;
        enemy_id_3 = -1;
    }

    if(enemy_properties.size() >= 2) {
        enemy_distance_2 = enemy_properties[1].distance;
        enemy_angle_2 = enemy_properties[1].angle;
        enemy_risk_2 = enemy_properties[1].risk_measure;
        enemy_id_2 = enemy_properties[1].id;
        enemy_properties.pop_back();
    } else {
        enemy_distance_2 = MAX_DISTANCE;
        enemy_angle_2 = 0;
        enemy_risk_2 = 0;
        enemy_id_2 = -1;
    }

    if(enemy_properties.size() >= 1) {
        enemy_distance_1 = enemy_properties[0].distance;
        enemy_angle_1 = enemy_properties[0].angle;
        enemy_risk_1 = enemy_properties[0].risk_measure;
        enemy_id_1 = enemy_properties[0].id;
        enemy_properties.pop_back();
    } else {
        enemy_distance_1 = MAX_DISTANCE;
        enemy_angle_1 = 0;
        enemy_risk_1 = 0;
        enemy_id_1 = -1;
    }
}

void observation::flattenObservationToVector(float (&observation_vector)[MAX_ABSTRACT_OBSERVATIONS]) {
    int nextPosOffset = 0;
    /// set trajectory. Takes 5 positions - on track, Left, Right, Front, off track
    // ONE HOT
    observation_vector[nextPosOffset++] = static_cast< float >(trajectory_on_track);
    observation_vector[nextPosOffset++] = static_cast< float >(trajectory_left);
    observation_vector[nextPosOffset++] = static_cast< float >(trajectory_right);
    observation_vector[nextPosOffset++] = static_cast< float >(trajectory_front);
    observation_vector[nextPosOffset++] = static_cast< float >(trajectory_off_track);

    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_front);
    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_front_left);
    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_blind_left);
    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_front_right);
    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_blind_right);

    observation_vector[nextPosOffset++] = enemy_distance_1;
    /// Angle is represented with ONE HOT.
    /// Enemies to the left have +ve angles and represented by 0 <value>
    /// Enemies to the right have -ve angles and represented by <value> 0
    /// Angle value decreases from front to back
    int offset = enemy_angle_1 > 0;
    observation_vector[nextPosOffset + offset] = abs(enemy_angle_1) * 10;
    nextPosOffset += 2;

    observation_vector[nextPosOffset++] = enemy_distance_2;
    offset = enemy_angle_2 > 0;
    observation_vector[nextPosOffset + offset] = abs(enemy_angle_2) * 10;
    nextPosOffset += 2;

    observation_vector[nextPosOffset++] = enemy_distance_3;
    offset = enemy_angle_3 > 0;
    observation_vector[nextPosOffset + offset] = abs(enemy_angle_3) * 10;
    nextPosOffset += 2;

}

void observation::locateRelativeTrajectory() {
    trajectory_on_track = trajectory == on_track;
    if(trajectory_on_track) {
        return;
    }
    trajectory_off_track = (direction == 0) and (trajectory == 0);
    if(trajectory_off_track) {
        return;
    }
    switch(trajectory) {
        case one_deviation_N:
            trajectory_front = direction == N;
            trajectory_left = (direction == E) or (direction == NE) or (direction == SE);
            trajectory_right = (direction != S) and (not trajectory_front) and (not trajectory_left);
            break;
        case one_deviation_S:
            trajectory_front = direction == S;
            trajectory_left = (direction == W) or (direction == NW) or (direction == SW);
            trajectory_right = (direction != N) and (not trajectory_front) and (not trajectory_left);
            break;
        case one_deviation_E:
            trajectory_front = direction == E;
            trajectory_left = (direction == S) or (direction == SW) or (direction == SE);
            trajectory_right = (direction != W) and (not trajectory_front) and (not trajectory_left);
            break;
        case one_deviation_W:
            trajectory_front = direction == W;
            trajectory_left = (direction == N) or (direction == NW) or (direction == NE);
            trajectory_right = (direction != E) and (not trajectory_front) and (not trajectory_left);
            break;

        case two_deviation_N:
            trajectory_front = direction == N;
            trajectory_left = (direction == E) or (direction == NE) or (direction == SE);
            trajectory_right = (direction != S) and (not trajectory_front) and (not trajectory_left);
            trajectory_front *= 2;
            trajectory_left *= 2;
            trajectory_right *= 2;
            break;
        case two_deviation_S:
            trajectory_front = direction == S;
            trajectory_left = (direction == W) or (direction == NW) or (direction == SW);
            trajectory_right = (direction != N) and (not trajectory_front) and (not trajectory_left);
            trajectory_front *= 2;
            trajectory_left *= 2;
            trajectory_right *= 2;
            break;
        case two_deviation_E:
            trajectory_front = direction == E;
            trajectory_left = (direction == S) or (direction == SW) or (direction == SE);
            trajectory_right = (direction != W) and (not trajectory_front) and (not trajectory_left);
            trajectory_front *= 2;
            trajectory_left *= 2;
            trajectory_right *= 2;
            break;
        case two_deviation_W:
            trajectory_front = direction == W;
            trajectory_left = (direction == N) or (direction == NW) or (direction == NE);
            trajectory_right = (direction != E) and (not trajectory_front) and (not trajectory_left);
            trajectory_front *= 2;
            trajectory_left *= 2;
            trajectory_right *= 2;
            break;
    }
    printRelativeTrajectory();
}

void observation::setGoalInSight(int probeX, int probeY) {
    isGoalInSight = (destinationX == probeX) and (destinationY == probeY);
}

void observation::findDestination() {
    if (isGoalInSight) {
        return;
    }
    // TODO: take from cnn
    for(int i = playerX - VISION_RADIUS; i<=playerX + VISION_RADIUS; i++) {
        for(int j = playerY - VISION_RADIUS; j<=playerY + VISION_RADIUS; j++) {
            if(i >= 0 and i < GRID_SPAN and j >= 0 and j < GRID_SPAN) {
                // FOV
                setGoalInSight(i, j);
                if (isGoalInSight) {
                    return;
                }
            }
        }
    }
}

void observation::printRelativeTrajectory() {

    if(trajectory_on_track) {
        logger->logDebug("Relative Trajectory ON Track")->endLineDebug();
    } else if(trajectory_front) {
        logger->logDebug("Relative Trajectory Front")->endLineDebug();
        if (trajectory_front == 2) {
            logger->logDebug("Two Deviations")->endLineDebug();
        }
    } else if (trajectory_left) {
        logger->logDebug("Relative Trajectory Left")->endLineDebug();
        if (trajectory_left == 2) {
            logger->logDebug("Two Deviations")->endLineDebug();
        }
    } else if (trajectory_right) {
        logger->logDebug("Relative Trajectory Right")->endLineDebug();
        if (trajectory_right == 2) {
            logger->logDebug("Two Deviations")->endLineDebug();
        }
    } else {
        logger->logDebug("Relative Trajectory OFF Track")->endLineDebug();
    }

}

void observation::printEnemyDistanceAndAngles() {
    if (enemy_distance_1 < MAX_DISTANCE) {
        logger->logDebug("Enemy1 Distance: ")->logDebug(enemy_distance_1)->endLineDebug();
        logger->logDebug("Enemy1 Angle: ")->logDebug(enemy_angle_1)->endLineDebug();
    }
    if (enemy_distance_2 < MAX_DISTANCE) {
        logger->logDebug("Enemy2 Distance: ")->logDebug(enemy_distance_2)->endLineDebug();
        logger->logDebug("Enemy2 Angle: ")->logDebug(enemy_angle_2)->endLineDebug();
    }
    if (enemy_distance_3 < MAX_DISTANCE) {
        logger->logDebug("Enemy3 Distance: ")->logDebug(enemy_distance_3)->endLineDebug();
        logger->logDebug("Enemy3 Angle: ")->logDebug(enemy_angle_3)->endLineDebug();
    }
}

void observation::recordFOVForCNN(CNN_controller& cnn) {
    cnn.populateFOVChannels(playerX, playerY, direction, obstaclesFOV, enemiesFOV);
}



