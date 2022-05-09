//
// Created by Debraj Ray on 2022-01-08.
//

#include "observation.h"
#include "coordinatesUtil.h"
#include "objectLocator.h"

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
    cout<<"Enemy distance and cosine "<<enemy_distance<<", "<<enemy_cosine<<endl;
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

void observation::locateTrajectoryAndDirection(const shared_ptr<findPath>& fp, int current_x, int current_y) {
    cout<<"locateTrajectoryAndDirection "<<endl;
    int row, col;
    for (int i=1; i<=VISION_RADIUS; i++) {
        row = current_x - i;
        if (row > 0) {
            for(col=current_y - i; col<=current_y + i; col++) {
                if (col > 0 && col < GRID_SPAN - 1) {
                    if (fp->isOnTrack(row, col)) {
                        direction = fp->pathDirection(row, col);
                        trajectory = (i*10) + 1;
                        return;
                    }
                }
            }
        }
        row = current_x + i;
        if (row < GRID_SPAN - 1) {
            for(col=current_y - i; col<=current_y + i; col++) {
                if (col > 0 && col < GRID_SPAN - 1) {
                    if (fp->isOnTrack(row, col)) {
                        direction = fp->pathDirection(row, col);
                        trajectory = (i*10) + 2;
                        return;
                    }
                }
            }
        }
        col = current_y - i;
        if (col > 0) {
            for(row=current_x - i; row<=current_x + i; row++) {
                if (row > 0 && row < GRID_SPAN - 1) {
                    if (fp->isOnTrack(row, col)) {
                        direction = fp->pathDirection(row, col);
                        trajectory = (i*10) + 3;
                        return;
                    }
                }
            }
        }
        col = current_y + i;
        if (col < GRID_SPAN - 1) {
            for(row=current_x - i; row<=current_x + i; row++) {
                if (row > 0 && row < GRID_SPAN - 1) {
                    if (fp->isOnTrack(row, col)) {
                        direction = fp->pathDirection(row, col);
                        trajectory = (i*10) + 4;
                        return;
                    }
                }
            }
        }
    }
}

void observation::locateEnemies(std::vector<enemy> &enemies, int current_x, int current_y) {
    objectLocator ol;
    for(const enemy& e: enemies) {
        ol.locateObject(current_x, current_y, direction, e.current_x, e.current_y);
        double distance = ol.getObjectDistance();
        if (distance <= VISION_RADIUS * sqrt(2)) {
            enemy_distance = distance;
            enemy_cosine = ol.getObjectCosine();
        } else {
            // reset
            enemy_distance = 5*VISION_RADIUS;
            enemy_cosine = -1; // cos theta
        }
    }
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



