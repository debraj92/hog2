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

    int countOfNodesToDestination;

    unordered_set<node_, node_::node_Hash> closedList;

    unordered_map<node_, node_, node_::node_Hash> childParent;

    const int GRID_SPAN_;

    void calculateAdjacentNodes(vector<pair<int, int>>& childNodes, node_& nextNode);

    float findShortestDistance(pair<int, int> src, pair<int, int> dst);

    bool isDestinationFound(node_ node);

    void finalizeNodeLinks();

    int reverseNodeLinks(node_& current);

    void addEdge(int src, int dest, vector<pair<int, int>> &nodes);

    double computeDistance(int x, int y);

    void reset();


public:

    /**
     * A star has temporary view of the map. This is done as the map is dynamic and therefore it is frozen for
     * A star. Since different units would be using this module they would populate the map with objects in their vision.
     * Such objects would not apply to other units.
     */
    AStar_(vector<vector<int>> &grid, int startX, int startY, int endX, int endY) : GRID_SPAN_(grid.size()) {
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

    void printBoard();

    int getCountOfNodesToDestination();

    void changeSourceAndDestination(int startX, int startY, int endX, int endY);

    void changeMap(vector<vector<int>> &grid);
};


#endif //EXAMPLE_ASTAR__H
