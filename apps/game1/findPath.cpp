//
// Created by Debraj Ray on 2021-12-30.
//

#include "findPath.h"
#include "AStar_.h"

using namespace std;

bool findPath::findPathToDestination() {
    return aStar.findPathToDestination();
}

void findPath::calculateNextPosition(int x, int y) {
    node_ current(x,y);
    if (isOnTrack(x, y)) {
        node_ next = aStar.getNextNode(current);
        next_x = next.x;
        next_y = next.y;
    } else {
        // stuck
        logger->logInfo("Unit Not on path, will get stuck")->endLineDebug();
        next_x = x;
        next_y = y;
    }

}

int findPath::getNext_x() {
    return next_x;
}

int findPath::getNext_y() {
    return next_y;
}

bool findPath::isOnTrack(int current_x, int current_y) {
    node_ current(current_x, current_y);
    return aStar.isOnPath(current);
}

/**
 *
 *     --->y (columns)
 *     |
 *     v
 *     x (rows)
 */

/**
*    NW  N  NE
*       |
*     W- -E
*       |
*  SW  S  SE
*/
int findPath::pathDirection(int x, int y) {
    node_ current(x,y);
    if (isOnTrack(x, y)) {
        node_ next = aStar.getNextNode(current);
        logger->logDebug("Current Position ")->logDebug(x)->logDebug(", ")->logDebug(y)->endLineDebug();
        logger->logDebug("Next Position ")->logDebug(next.x)->logDebug(", ")->logDebug(next.y)->endLineDebug();
        int x_plus = x+1;
        int x_minus = x-1;
        int y_plus = y+1;
        int y_minus = y-1;
        if (x_plus == next.x && y_plus == next.y) {
            return SE;
        } else if (x == next.x && y_plus == next.y) {
            return E;
        } else if (x_minus == next.x && y_plus == next.y) {
            return NE;
        } else if (x_minus == next.x && y == next.y) {
            return N;
        } else if (x_minus == next.x && y_minus == next.y) {
            return NW;
        } else if (x == next.x && y_minus == next.y) {
            return W;
        } else if (x_plus == next.x && y_minus == next.y) {
            return SW;
        } else if (x_plus == next.x && y == next.y) {
            return S;
        }
        // TODO: Error
        logger->logInfo("Invalid next move")->endLineDebug();
        return -1;
    } else {
        // TODO: Error Handling
        logger->logInfo("Unit not on track, cannot find direction")->endLineDebug();
    }
    return -1;
}

void findPath::populateEnemyObstacles(vector<enemy> &enemies) {
    aStar.populateEnemyObstacles(enemies);
}

int findPath::getDistanceToDestination() {
    return aStar.getCountOfNodesToDestination();
}

void findPath::printMap() {
    aStar.printBoard();
}

void findPath::changeSourceAndDestination(int startX, int startY, int endX, int endY) {
    aStar.changeSourceAndDestination(startX, startY, endX, endY);
}

void findPath::changeMap(vector<vector<int>> &grid) {
    aStar.changeMap(grid);
}
