//
// Created by Debraj Ray on 2021-12-30.
//

#include "findPath.h"
#include "AStar_.h"

using namespace std;

bool findPath::findPathToDestination(int destinationDirection) {
    logger->logDebug("findPath::findPathToDestination")->endLineDebug();
    bool pathFound = aStar.findPathToDestination(destinationDirection);
    visited_x_onpath = -1;
    visited_y_onpath = -1;
    return pathFound;
}

void findPath::calculateNextPosition() {
    node_ current(visited_x_onpath, visited_y_onpath);
    node_ next = aStar.getNextNode(current);
    next_x = next.x;
    next_y = next.y;
}

int findPath::getNext_x() {
    return next_x;
}

int findPath::getNext_y() {
    return next_y;
}

bool findPath::isOnTrack(int current_x, int current_y) {
    node_ current(current_x, current_y);
    bool ontrack = aStar.isOnPath(current);
    if (ontrack) {
        if (visited_x_onpath == -1) {
            visited_x_onpath = current_x;
            visited_y_onpath = current_y;
            return true;
        }
        if (compareNodeOrders(current_x, current_y, visited_x_onpath, visited_y_onpath) > 0) {
            visited_x_onpath = current_x;
            visited_y_onpath = current_y;
            return true;
        }
    }
    return false;
}

bool findPath::isOnTrackNoMemorizing(int current_x, int current_y) {
    node_ current(current_x, current_y);
    bool ontrack = aStar.isOnPath(current);
    if (visited_x_onpath == -1) {
        return ontrack;
    }
    return ontrack and (compareNodeOrders(current_x, current_y, visited_x_onpath, visited_y_onpath) > 0);
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
/// X and Y must be on track
int findPath::pathDirection(int x, int y) {
    node_ current(x,y);
    node_ next = aStar.getNextNode(current);
    logger->logDebug("Current Position ")->logDebug(x)->logDebug(", ")->logDebug(y)->endLineDebug();
    logger->logDebug("Next Position ")->logDebug(next.x)->logDebug(", ")->logDebug(next.y)->endLineDebug();
    return inferDirection(x, y, next.x, next.y);
}

int findPath::getDistanceToDestination() {
    return aStar.getCountOfNodesToDestination();
}

void findPath::changeSourceAndDestination(int startX, int startY, int endX, int endY) {
    aStar.changeSourceAndDestination(startX, startY, endX, endY);
}

void findPath::changeMap(vector<vector<int>> &grid) {
    aStar.changeMap(grid);
}

int findPath::compareNodeOrders(int location1_x, int location1_y, int location2_x, int location2_y) {
    node_ first(location1_x, location1_y);
    node_ second(location2_x, location2_y);
    return aStar.compareNodeOrders(first, second);
}

void findPath::getNextPositionAfterGivenLocation(int &given_x, int &given_y, int &next_x, int &next_y) {
    node_ current(given_x, given_y);
    node_ next = aStar.getNextNode(current);
    next_x = next.x;
    next_y = next.y;
}

int findPath::findDistanceToGoal(int &x, int &y) {
    node_ givenLocation(x, y);
    return aStar.getTotalDistanceToDestination() - aStar.getNodeOrder(givenLocation);
}

int findPath::getNodeOrder(int &x, int &y) {
    node_ givenLocation(x, y);
    return aStar.getNodeOrder(givenLocation);
}

float findPath::getStraightLineDistanceToDestination() {
    return aStar.findShortestDistanceEuclidean({current_x, current_y}, {destination_x, destination_y});
}

float findPath::getShortestDistanceToDestination() {
    return aStar.findShortestDistance({current_x, current_y}, {destination_x, destination_y});
}

int findPath::inferDirection(int x, int y, int next_x, int next_y) {
    logger->logDebug("findPath::inferDirection")->endLineDebug();
    if (x == next_x and y == next_y) {
        return 0;
    }
    int x_plus = x + 1;
    int x_minus = x - 1;
    int y_plus = y + 1;
    int y_minus = y - 1;
    if (x_plus == next_x && y_plus == next_y) {
        return SE;
    } else if (x == next_x && y_plus == next_y) {
        return E;
    } else if (x_minus == next_x && y_plus == next_y) {
        return NE;
    } else if (x_minus == next_x && y == next_y) {
        return N;
    } else if (x_minus == next_x && y_minus == next_y) {
        return NW;
    } else if (x == next_x && y_minus == next_y) {
        return W;
    } else if (x_plus == next_x && y_minus == next_y) {
        return SW;
    } else if (x_plus == next_x && y == next_y) {
        return S;
    }
    logger->logInfo("ERROR : Invalid pathDirection")->endLineInfo();
    return -1;
}
