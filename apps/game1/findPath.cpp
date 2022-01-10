//
// Created by Debraj Ray on 2021-12-30.
//

#include "findPath.h"

void findPath::findPathToDestination() {
    // TODO
    for(int i=0; i<GRID_SPAN; i++) {
        pair<int, int> vertex = make_pair(i,i);
        path.push_back(vertex);
        all_vertices.insert(make_pair(vertex,SE));
    }
}

void findPath::calculateNextPosition(int x, int y) {
    // TODO: check if x and y are part of path
    current_x = x;
    current_y = y;
    pointer++;
    next_x = path[pointer].first;
    next_y = path[pointer].second;
}

int findPath::getNext_x() {
    return next_x;
}

int findPath::getNext_y() {
    return next_y;
}

void findPath::updateCurrentCoordinates(int x, int y) {
    current_x = x;
    current_y = y;
}

void findPath::reset() {
    path.clear();
    all_vertices.clear();
    pointer = 0;
}
