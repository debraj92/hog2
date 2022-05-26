//
// Created by Debraj Ray on 2022-02-12.
//

#ifndef EXAMPLE_ASTAR__H
#define EXAMPLE_ASTAR__H

#include <vector>
#include <unordered_set>
#include "node_.h"
#include <unordered_map>
#include "enemy/enemy.h"
#include "iostream"

using namespace std;

class AStar_ {

    vector<vector<int>> grid;
    pair<int, int> source;
    pair<int, int> destination;

    unordered_set<node_, node_::node_Hash> closedList;

    unordered_map<node_, node_, node_::node_Hash> childParent;

    void calculateAdjacentNodes(vector<pair<int, int>>& childNodes, node_& nextNode);

    float findShortestDistance(pair<int, int> src, pair<int, int> dst);

    bool isDestinationFound(node_ node);

    void finalizeNodeLinks();

    int reverseNodeLinks(node_& current);

    void addEdge(int src, int dest, vector<pair<int, int>> &nodes);

    double computeDistance(int x, int y);

public:

    AStar_(vector<vector<int>> &grid, int startX, int startY, int endX, int endY) {
        cout<<"AStar created, source: "<<startX<<", "<<startY<<endl;
        std::copy(grid.begin(), grid.end(), back_inserter(this->grid));
        source = make_pair(startX, startY);
        destination = make_pair(endX, endY);
    }

    bool findPathToDestination();

    node_ getNextNode(node_& current);

    bool isOnPath(node_& current);

    void populateEnemyObstacles(vector<enemy> &enemies);

    void printTrack(node_ root);

    void printBoard(vector<std::vector<int>> &grid);
};


#endif //EXAMPLE_ASTAR__H
