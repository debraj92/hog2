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

    // left
    next_x = x;
    next_y = y;

    for(int i=1; i<=VISION_RADIUS; i++) {
        if (coordinates.setDodgeLeftActionCoordinates(next_x, next_y, direction) == -1) {
            obstacle_left = i;
            break;
        }
    }

    // front-left
    next_x = x;
    next_y = y;

    for(int i=1; i<=VISION_RADIUS; i++) {
        if (coordinates.setDodgeDiagonalLeftActionCoordinates(next_x, next_y, direction) == -1) {
            obstacle_front_left = i;
            break;
        }
    }

    // right
    next_x = x;
    next_y = y;

    for(int i=1; i<=VISION_RADIUS; i++) {
        if (coordinates.setDodgeRightActionCoordinates(next_x, next_y, direction) == -1) {
            obstacle_right = i;
            break;
        }
    }

    // front-right
    next_x = x;
    next_y = y;

    for(int i=1; i<=VISION_RADIUS; i++) {
        if (coordinates.setDodgeDiagonalRightActionCoordinates(next_x, next_y, direction) == -1) {
            obstacle_front_right = i;
            break;
        }
    }
}

void observation::printData() {
    cout<<"Player Direction "<<direction<<endl;
    cout<<"Player trajectory "<<trajectory<<endl;
    //cout<<"Enemy distance and cosine "<<enemy_distance<<", "<<enemy_cosine<<endl;
    cout<<"Obstacle front "<<obstacle_front<<endl;
    cout<<"Obstacle left "<<obstacle_left<<endl;
    cout<<"Obstacle front left "<<obstacle_front_left<<endl;
    cout<<"Obstacle right "<<obstacle_right<<endl;
    cout<<"Obstacle front right "<<obstacle_front_right<<endl;
}

/**
 *   -->y
 *   |
 *   v
 *   x
 */

void observation::locateTrajectoryAndDirection(const shared_ptr<findPath>& fp, int destination_x, int destination_y) {
    cout<<"locateTrajectoryAndDirection "<<endl;

    int current_x = playerX;
    int current_y = playerY;
    if (fp->isOnTrack(current_x, current_y)) {
        trajectory = on_track;
        direction = fp->pathDirection(current_x, current_y);
        return;
    }

    int angles[9];
    setDirectionAngles(angles);
    redirect(current_x, current_y, destination_x, destination_y);
    bool pathFound = false;
    int direction_dest = direction;
    int min_angle = 1000;
    int row, col;
    for (int i=1; i<=VISION_RADIUS; i++) {
        row = current_x - i;
        if (row >= 0) {
            for(col=current_y - i; col<=current_y + i; col++) {
                if (col >= 0 && col <= GRID_SPAN - 1) {
                    if (fp->isOnTrack(row, col)) {
                        direction = fp->pathDirection(row, col);
                        trajectory = (i*10) + 1;
                        int diff = abs(angles[direction_dest] - angles[direction]);
                        min_angle = diff;
                        pathFound = true;
                        break;
                    }
                }
            }
        }
        row = current_x + i;
        if (row <= GRID_SPAN - 1) {
            for(col=current_y - i; col<=current_y + i; col++) {
                if (col >= 0 && col <= GRID_SPAN - 1) {
                    if (fp->isOnTrack(row, col)) {
                        if (min_angle == 1000) {
                            direction = fp->pathDirection(row, col);
                            trajectory = (i*10) + 2;
                            int diff = abs(angles[direction_dest] - angles[direction]);
                            min_angle = diff;
                        } else {
                            int temp_direction = fp->pathDirection(row, col);
                            int diff = abs(angles[direction_dest] - angles[temp_direction]);
                            if (diff < min_angle) {
                                min_angle = diff;
                                direction = temp_direction;
                                trajectory = (i*10) + 2;
                            }
                        }
                        pathFound = true;
                        break;
                    }
                }
            }
        }
        col = current_y - i;
        if (col >= 0) {
            for(row=current_x - i; row<=current_x + i; row++) {
                if (row >= 0 && row <= GRID_SPAN - 1) {
                    if (fp->isOnTrack(row, col)) {
                        if (min_angle == 1000) {
                            direction = fp->pathDirection(row, col);
                            trajectory = (i * 10) + 3;
                            int diff = abs(angles[direction_dest] - angles[direction]);
                            min_angle = diff;
                        } else {
                            int temp_direction = fp->pathDirection(row, col);
                            int diff = abs(angles[direction_dest] - angles[temp_direction]);
                            if (diff < min_angle) {
                                min_angle = diff;
                                direction = temp_direction;
                                trajectory = (i * 10) + 3;
                            }
                        }
                        pathFound = true;
                        break;
                    }
                }
            }
        }
        col = current_y + i;
        if (col <= GRID_SPAN - 1) {
            for(row=current_x - i; row<=current_x + i; row++) {
                if (row >= 0 && row <= GRID_SPAN - 1) {
                    if (fp->isOnTrack(row, col)) {
                        if (min_angle == 1000) {
                            direction = fp->pathDirection(row, col);
                            trajectory = (i * 10) + 4;
                        } else {
                            int temp_direction = fp->pathDirection(row, col);
                            int diff = abs(angles[direction_dest] - angles[temp_direction]);
                            if (diff < min_angle) {
                                direction = temp_direction;
                                trajectory = (i * 10) + 4;
                            }
                        }
                        pathFound = true;
                        break;
                    }
                }
            }
        }
        if(pathFound) {
            return;
        }
    }
    trajectory = 0;
    direction = 0;
}

void observation::locateEnemies(std::vector<enemy> &enemies) {
    objectLocator ol;
    vector<enemy_attributes> enemy_properties;
    for(const enemy& e: enemies) {
        ol.locateObject(playerX, playerY, direction, e.current_x, e.current_y);
        double distance = ol.getObjectDistance();
        if (distance <= VISION_RADIUS * sqrt(2)) {
            enemy_properties.push_back({ol.getObjectDistance(), ol.getObjectAngle(), ol.getObjectRiskFeature()});
        }
    }

    cout<<"Number of enemies in vision "<<enemy_properties.size()<<endl;

    sort(enemy_properties.begin(), enemy_properties.end(),
         [](const enemy_attributes &e1, const enemy_attributes &e2) -> bool
         {
            return e1.risk_measure > e2.risk_measure;
         });

    updateEnemyDistanceAndAngles(enemy_properties);

}

void observation::redirect(int current_x, int current_y, int destination_x, int destination_y) {
    cout<<"observation::redirect "<<endl;
    using namespace xt;
    double dx = destination_x - current_x;
    double dy = destination_y - current_y;
    double magnitude_d = sqrt(pow(dx,2) + pow(dy,2));
    xarray<double>::shape_type shape = {9};
    xarray<double> angles = xarray<double>::from_shape(shape);;
    angles[0] = 1000;
    angles[N] = acos(-dx / magnitude_d);
    angles[S] = acos(dx / magnitude_d);
    angles[E] = acos(dy / magnitude_d);
    angles[W] = acos(-dy / magnitude_d);

    angles[NE] = acos((-dx + dy) / (magnitude_d * sqrt(2)));
    angles[NW] = acos((-dx - dy) / (magnitude_d * sqrt(2)));
    angles[SE] = acos((dx + dy) / (magnitude_d * sqrt(2)));
    angles[SW] = acos((dx - dy) / (magnitude_d * sqrt(2)));

    direction = argmin(angles)[0];
}

void observation::setDirectionAngles(int (&angles)[9]) {
    angles[N] = N_Angle;
    angles[S] = S_Angle;
    angles[E] = E_Angle;
    angles[W] = W_Angle;
    angles[NE] = NE_Angle;
    angles[NW] = NW_Angle;
    angles[SE] = SE_Angle;
    angles[SW] = SW_Angle;
}

void observation::updateEnemyDistanceAndAngles(vector<enemy_attributes>& enemy_properties) {
    // Ignoring enemy count above 4
    if(enemy_properties.size() >= 4) {
        enemy_distance_4 = enemy_properties[3].distance;
        enemy_angle_4 = enemy_properties[3].angle;
        enemy_risk_4 = enemy_properties[3].risk_measure;
        enemy_properties.pop_back();
    } else {
        enemy_distance_4 = MAX_DISTACE;
        enemy_angle_4 = -1; // cos theta
        enemy_risk_4 = 0;
    }

    if(enemy_properties.size() == 3) {
        enemy_distance_3 = enemy_properties[2].distance;
        enemy_angle_3 = enemy_properties[2].angle;
        enemy_risk_3 = enemy_properties[2].risk_measure;
        enemy_properties.pop_back();
    } else {
        enemy_distance_3 = MAX_DISTACE;
        enemy_angle_3 = -1; // cos theta
        enemy_risk_3 = 0;
    }

    if(enemy_properties.size() == 2) {
        enemy_distance_2 = enemy_properties[1].distance;
        enemy_angle_2 = enemy_properties[1].angle;
        enemy_risk_2 = enemy_properties[1].risk_measure;
        enemy_properties.pop_back();
    } else {
        enemy_distance_2 = MAX_DISTACE;
        enemy_angle_2 = -1; // cos theta
        enemy_risk_2 = 0;
    }

    if(enemy_properties.size() == 1) {
        enemy_distance_1 = enemy_properties[0].distance;
        enemy_angle_1 = enemy_properties[0].angle;
        enemy_risk_1 = enemy_properties[0].risk_measure;
        enemy_properties.pop_back();
    } else {
        enemy_distance_1 = MAX_DISTACE;
        enemy_angle_1 = -1; // cos theta
        enemy_risk_1 = 0;
    }
}

void observation::resetRerouteDistance() {
    rerouteDistance = 1000;
}

/*
void observation::flattenObservationToVector(float (&observation_vector)[MAX_ABSTRACT_OBSERVATIONS]) {
    int nextPosOffset = 0;
    // ONE HOT
    if (direction > 0) {
        // set direction [first 8 positions taken]
        observation_vector[direction % 8] = 1;
    }
    nextPosOffset += 8;
    // set trajectory. Takes 9 positions
    // ONE HOT
    if (trajectory >= 10) {
        if (trajectory < 20) {
            // indices : 0, 1, 2, 3, 4
            observation_vector[nextPosOffset + trajectory - 10] = 1;
        } else {
            // indices : 5, 6, 7, 8
            observation_vector[nextPosOffset + trajectory - 20 + 4] = 1;
        }
    }
    nextPosOffset += 9;
    // Real Number values next
    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_front);
    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_left);
    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_right);
    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_front_left);
    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_front_right);

    observation_vector[nextPosOffset++] = static_cast< float >(rerouteDistance);

    observation_vector[nextPosOffset++] = enemy_distance_1;
    observation_vector[nextPosOffset++] = enemy_angle_1;
    observation_vector[nextPosOffset++] = enemy_risk_1;

    observation_vector[nextPosOffset++] = enemy_distance_2;
    observation_vector[nextPosOffset++] = enemy_angle_2;
    observation_vector[nextPosOffset++] = enemy_risk_2;

    observation_vector[nextPosOffset++] = enemy_distance_3;
    observation_vector[nextPosOffset++] = enemy_angle_3;
    observation_vector[nextPosOffset++] = enemy_risk_3;

    observation_vector[nextPosOffset++] = enemy_distance_4;
    observation_vector[nextPosOffset++] = enemy_angle_4;
    observation_vector[nextPosOffset] = enemy_risk_4;

}
 */

void observation::flattenObservationToVector(float (&observation_vector)[MAX_ABSTRACT_OBSERVATIONS]) {
    int nextPosOffset = 0;

    // ONE HOT
    if (direction > 0) {
        // set direction [first 8 positions taken]
        observation_vector[direction % 8] = 1;
    }
    nextPosOffset += 8;
    // set trajectory. Takes 9 positions
    // ONE HOT
    if (trajectory >= 10) {
        if (trajectory < 20) {
            // indices : 0, 1, 2, 3, 4
            observation_vector[nextPosOffset + trajectory - 10] = 1;
        } else {
            // indices : 5, 6, 7, 8
            observation_vector[nextPosOffset + trajectory - 20 + 4] = 1;
        }
    }
    nextPosOffset += 9;

    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_front);
    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_left);
    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_right);
    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_front_left);
    observation_vector[nextPosOffset++] = static_cast< float >(obstacle_front_right);

}



