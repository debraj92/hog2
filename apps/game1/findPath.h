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

using namespace std;

class findPath {

    int current_x;
    int current_y;
    int next_x;
    int next_y;

    int destination_x;
    int destination_y;

    vector<std::pair<int, int>> path;
    int pointer = 0;

public:

    std::map<std::pair<int, int>, int> all_vertices;

    findPath(int x, int y, int dest_x, int dest_y){
        current_x = x;
        current_y = y;
        destination_x = dest_x;
        destination_y = dest_y;
    }
    void findPathToDestination();
    void calculateNextPosition(int x, int y);
    int getNext_x();
    int getNext_y();
    void updateCurrentCoordinates(int x, int y);
    void reset();
};


#endif //EXAMPLE_FINDPATH_H
