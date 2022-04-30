//
// Created by Debraj Ray on 2022-02-12.
//

#ifndef EXAMPLE_ASTAR__H
#define EXAMPLE_ASTAR__H

#include <vector>
#include <unordered_set>
#include "node_.h"
#include <unordered_map>

using namespace std;

class AStar_ {

    vector<vector<int>> &grid;
    pair<int, int> source;
    pair<int, int> destination;

    unordered_set<node_, node_::node_Hash> closedList;

    unordered_map<node_, node_, node_::node_Hash> childParent;

    void calculateAdjacentNodes(vector<pair<int, int>>& childNodes, node_& nextNode);

    float findShortestDistance(pair<int, int> src, pair<int, int> dst);

    bool isDestinationFound(node_ node);

    void finalizeNodeLinks();

    void reverseNodeLinks();

public:

    AStar_(vector<vector<int>> &grid, int startX, int startY, int endX, int endY) : grid(grid) {
        source = make_pair(startX, startY);
        destination = make_pair(endX, endY);
    }

    bool findPathToDestination();

    int countTotalNodesInOptimalPath(node_& current);

    node_ getNextNode(node_& current);

    bool isOnPath(node_& current);

};


#endif //EXAMPLE_ASTAR__H
