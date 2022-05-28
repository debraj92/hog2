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
void observation::updateObstacleDistances(std::vector <std::vector<int>> &grid, int x, int y) {
    coordinatesUtil coordinates(grid);

    // TODO: Consider backward movements?

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

void observation::locateTrajectoryAndDirection(const shared_ptr<findPath>& fp, int current_x, int current_y, int destination_x, int destination_y) {
    cout<<"locateTrajectoryAndDirection "<<endl;
    int angles[9];
    setDirectionAngles(angles);
    redirect(current_x, current_y, destination_x, destination_y);
    int direction_dest = direction;
    int min_angle = 1000;
    int row, col;
    for (int i=1; i<=VISION_RADIUS; i++) {
        row = current_x - i;
        if (row > 0) {
            for(col=current_y - i; col<=current_y + i; col++) {
                if (col > 0 && col < GRID_SPAN - 1) {
                    if (fp->isOnTrack(row, col)) {
                        direction = fp->pathDirection(row, col);
                        trajectory = (i*10) + 1;
                        int diff = abs(angles[direction_dest] - angles[direction]);
                        min_angle = diff;
                        break;
                    }
                }
            }
        }
        row = current_x + i;
        if (row < GRID_SPAN - 1) {
            for(col=current_y - i; col<=current_y + i; col++) {
                if (col > 0 && col < GRID_SPAN - 1) {
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
                        break;
                    }
                }
            }
        }
        col = current_y - i;
        if (col > 0) {
            for(row=current_x - i; row<=current_x + i; row++) {
                if (row > 0 && row < GRID_SPAN - 1) {
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
                        break;
                    }
                }
            }
        }
        col = current_y + i;
        if (col < GRID_SPAN - 1) {
            for(row=current_x - i; row<=current_x + i; row++) {
                if (row > 0 && row < GRID_SPAN - 1) {
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
                        break;
                    }
                }
            }
        }
        if (trajectory != 0) {
            return;
        }
    }
    if (trajectory == 0) {
        // reset if path not found
        direction = 0;
    }
}

void observation::locateEnemies(std::vector<enemy> &enemies, int current_x, int current_y) {
    objectLocator ol;
    vector<enemy_distance_cosine> enemy_distance_cosines;
    for(const enemy& e: enemies) {
        ol.locateObject(current_x, current_y, direction, e.current_x, e.current_y);
        double distance = ol.getObjectDistance();
        cout<<"Enemy cosine "<<ol.getObjectCosine()<<endl;
        if (distance <= VISION_RADIUS * sqrt(2)) {
            enemy_distance_cosine edc = {distance, ol.getObjectCosine()};
            enemy_distance_cosines.push_back(edc);
            enemies_in_vision.push_back(e);
        }
    }

    cout<<"Number of enemies in vision "<<enemy_distance_cosines.size()<<endl;

    sort(enemy_distance_cosines.begin(), enemy_distance_cosines.end(),
         [](const enemy_distance_cosine &e1, const enemy_distance_cosine &e2) -> bool
         {
            if (abs(e1.cosine - e2.cosine) <= 0.5) {
                return e1.distance > e2.distance;
            } else {
                return e1.cosine > e2.cosine;
            }
         });

    updateEnemyDistanceAndAngles(enemy_distance_cosines);

}

// This is probably not required because the goal of RL is to tackle adversary and restore to A star track.
void observation::locateDestination(int current_x, int current_y, int destination_x, int destination_y) {
    objectLocator ol;
    ol.locateObject(current_x, current_y, direction, destination_x, destination_y);
    double distance = ol.getObjectDistance();
    if (distance <= VISION_RADIUS * sqrt(2)) {
        destination_distance = distance;
        destination_cosine = ol.getObjectCosine();
    } else {
        destination_distance = 1000;
        destination_cosine = -1;
    }
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

void observation::updateEnemyDistanceAndAngles(vector<enemy_distance_cosine>& enemy_distance_cosines) {
    // Ignoring enemy count above 4
    if(enemy_distance_cosines.size() >= 4) {
        enemy_distance_4 = enemy_distance_cosines[3].distance;
        enemy_cosine_4 = enemy_distance_cosines[3].cosine;
        enemy_distance_cosines.pop_back();
    } else {
        enemy_distance_4 = 5*VISION_RADIUS;
        enemy_cosine_4 = -1; // cos theta
    }

    if(enemy_distance_cosines.size() == 3) {
        enemy_distance_3 = enemy_distance_cosines[2].distance;
        enemy_cosine_3 = enemy_distance_cosines[2].cosine;
        enemy_distance_cosines.pop_back();
    } else {
        enemy_distance_3 = 5*VISION_RADIUS;
        enemy_cosine_3 = -1; // cos theta
    }

    if(enemy_distance_cosines.size() == 2) {
        enemy_distance_2 = enemy_distance_cosines[1].distance;
        enemy_cosine_2 = enemy_distance_cosines[1].cosine;
        enemy_distance_cosines.pop_back();
    } else {
        enemy_distance_2 = 5*VISION_RADIUS;
        enemy_cosine_2 = -1; // cos theta
    }

    if(enemy_distance_cosines.size() == 1) {
        enemy_distance_1 = enemy_distance_cosines[0].distance;
        enemy_cosine_1 = enemy_distance_cosines[0].cosine;
        enemy_distance_cosines.pop_back();
    } else {
        enemy_distance_1 = 5*VISION_RADIUS;
        enemy_cosine_1 = -1; // cos theta
    }
}

void observation::resetRerouteDistance() {
    rerouteDistance = 1000;
}



