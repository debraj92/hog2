//
// Created by Debraj Ray on 2022-01-08.
//

#include "observation.h"
#include "coordinatesUtil.h"

using namespace std;
void observation::updateObstacleDistances(std::vector <std::vector<int>> &grid, int x, int y) {
    coordinatesUtil coordinates;

    // TODO: Check obstacles [Consider backward movements?]

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

