//
// Created by Debraj Ray on 2021-12-30.
//

#ifndef EXAMPLE_FINDPATH_H
#define EXAMPLE_FINDPATH_H

#include <iostream>
#include "vector"
#include <set>
#include <map>
#include "gameConstants.h"
#include "AStar_.h"
#include "enemy/enemy.h"

using namespace std;

class findPath {

    int current_x;
    int current_y;
    int next_x;
    int next_y;
    AStar_ aStar;

    int destination_x;
    int destination_y;

    vector<std::pair<int, int>> path;

public:

    findPath(std::vector<std::vector<int>> &grid, int src_x, int src_y, int dest_x, int dest_y) : aStar(grid, src_x, src_y, dest_x, dest_y){
        current_x = src_x;
        current_y = src_y;
        destination_x = dest_x;
        destination_y = dest_y;
    }
    void findPathToDestination();
    void calculateNextPosition(int x, int y);
    int getNext_x();
    int getNext_y();
    bool isOnTrack(int current_x, int current_y);
    int pathDirection(int current_x, int current_y);
    void populateEnemyObstacles(vector<enemy> &enemies);
};


#endif //EXAMPLE_FINDPATH_H
