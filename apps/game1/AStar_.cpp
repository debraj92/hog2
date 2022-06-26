//
// Created by Debraj Ray on 2022-02-12.
//

#include "AStar_.h"
#include "gameConstants.h"
#include "node_.h"
#include "AStarOpenList.h"
#include <vector>
#include <iostream>

using namespace std;

float AStar_::findShortestDistance(pair<int, int> src, pair<int, int> dst) {
    float x_sqr = pow((src.first - dst.first),2);
    float y_sqr = pow((src.second - dst.second),2);
    return sqrt(x_sqr + y_sqr);
}

bool AStar_::findPathToDestination() {
    logger->printBoardDebug(grid);
    reset();
    node_ root(source.first, source.second);
    root.computeF(0, findShortestDistance(source, destination));
    childParent.insert(make_pair(root, root));
    AStarOpenList openList;
    openList.insert(root);
    while(!openList.isEmpty()) {
        node_ nextNode = openList.removeMinimum();
        closedList.insert(nextNode);
        if(isDestinationFound(nextNode)) {
            finalizeNodeLinks();
            countOfNodesToDestination = reverseNodeLinks(nextNode);
            logger->logDebug("Number of nodes to destination ")->logDebug(countOfNodesToDestination)->endLineDebug();
            orderNodeLinks(root);
            eraseDestinationNode();
            printTrack(root);
            return true;
        }
        vector<pair<int, int>> childNodes;
        calculateAdjacentNodes(childNodes, nextNode);
        for (pair<int, int> node_pair : childNodes) {
            node_ temp(node_pair.first, node_pair.second);
            if(closedList.find(temp) !=  closedList.end()) {
                // Don't expand already expanded nodes
                continue;
            }
            if(!openList.isPresent(temp)) {
                // insert (parent: next closest node and child: reachable node)
                childParent.insert(make_pair(temp, nextNode));
                temp.computeF(nextNode.g + 1, findShortestDistance(node_pair, destination));
                openList.insert(temp);
            } else {
                if (openList.updateIfBetterPath(temp, nextNode.g + 1)) {
                    childParent.insert(make_pair(temp, nextNode));
                }
            }
        }

    }
    return false;

}

void AStar_::calculateAdjacentNodes(vector<pair<int, int>> &childNodes, node_& nextNode) {
    /**
             * 8 possible moves
             * i-1, j-1
             * i-1, j
             * i-1, j+1
             * i,j-1
             * i, j+1
             * i+1,j-1
             * i+1,j
             * i+1, j+1
             */

    if(nextNode.x-1 >= 0) {
        //j-1, j, j+1 : 3 moves
        if (nextNode.y-1 >= 0) {
            addEdge(nextNode.x-1, nextNode.y-1, childNodes);
        }
        if (nextNode.y+1 < GRID_SPAN_) {
            addEdge(nextNode.x-1, nextNode.y+1, childNodes);
        }
        addEdge(nextNode.x-1, nextNode.y, childNodes);
    }
    if(nextNode.x+1 < GRID_SPAN_) {
        //j-1, j, j+1 : 3 moves
        if (nextNode.y-1 >= 0) {
            addEdge(nextNode.x+1, nextNode.y-1, childNodes);
        }
        if (nextNode.y+1 < GRID_SPAN_) {
            addEdge(nextNode.x+1, nextNode.y+1, childNodes);
        }
        addEdge(nextNode.x+1, nextNode.y, childNodes);
    }
    if (nextNode.y-1 >= 0) {
        // i : 1 move
        addEdge(nextNode.x, nextNode.y-1, childNodes);
    }
    if (nextNode.y+1 < GRID_SPAN_) {
        // i : 1 move
        addEdge(nextNode.x, nextNode.y+1, childNodes);
    }
}

bool AStar_::isDestinationFound(node_ node) {
    return node.x == destination.first and node.y == destination.second;
}

node_ AStar_::getNextNode(node_& current) {
    if(isDestinationFound(current)) {
        logger->logInfo("Next node after destination is destination (loop?)");
        return current;
    }
    return childParent.find(current)->second;
}

bool AStar_::isOnPath(node_& current) {
    return childParent.find(current) != childParent.end();
}

void AStar_::finalizeNodeLinks() {
    auto childParentIterator = childParent.begin();
    while(childParentIterator != childParent.end()) {
        node_ node = childParentIterator->first;
        if (closedList.find(node) == closedList.end()) {
            childParentIterator++;
            childParent.erase(node);
        } else {
            childParentIterator++;
        }
    }
}

int AStar_::reverseNodeLinks(node_& current) {
    unordered_map<node_, node_, node_::node_Hash> temp_childParent;
    node_ parent = childParent.find(current)->second;
    temp_childParent.insert(make_pair(parent, current));
    int count = 0;
    while(parent !=  current) {
        current = parent;
        parent = childParent.find(parent)->second;
        temp_childParent.insert(make_pair(parent, current));
        count++;
    }
    childParent = temp_childParent;
    return count+1;
}

void AStar_::addEdge(int src, int dest, vector<pair<int, int>> &nodes) {
    if (grid[src][dest] >= 0) {
        nodes.emplace_back(src, dest);
    }
}

double AStar_::computeDistance(int x, int y) {
    double d1 = x - source.first;
    double d2 = y - source.second;
    return sqrt(pow(d1, 2) + pow(d2, 2));
}

void AStar_::populateEnemyObstacles(vector<enemy> &enemies) {
    for(const enemy& e: enemies) {
        if (computeDistance(e.current_x, e.current_y) <= VISION_RADIUS * sqrt(2)) {
            grid[e.current_x][e.current_y] = -e.id;
        }
    }
}

void AStar_::printTrack(node_ root) {
    logger->logDebug("AStar_::printTrack")->endLineDebug();
    auto current = childParent.find(root)->first;
    string path = "("+ to_string(current.x)+", "+to_string(current.y)+") ";
    path += "["+ to_string(current.order)+"] ";
    while(!isDestinationFound(current)) {
        current = getNextNode(current);
        path += "("+ to_string(current.x)+", "+to_string(current.y)+") ";
        path += "["+ to_string(current.order)+"] ";
    }
    logger->logDebug(path)->endLineDebug();
}

// TODO: Delete after removing calls from tests
void AStar_::printBoard() {
    cout<<"AStar_::print board"<<endl;
    for (int row=0; row<GRID_SPAN_; row++) {
        for (int col=0; col<GRID_SPAN_; col++) {
            if(grid[row][col]<0) {
                cout<<grid[row][col]<<" ";
            } else {
                cout<<" "<<grid[row][col]<<" ";
            }
        }
        cout<<"\n";
    }
}

int AStar_::getCountOfNodesToDestination() {
    return countOfNodesToDestination;
}

void AStar_::reset() {
    countOfNodesToDestination = 0;
    closedList.clear();
    childParent.clear();
}

void AStar_::changeSourceAndDestination(int startX, int startY, int endX, int endY) {
    source = make_pair(startX, startY);
    destination = make_pair(endX, endY);
}

void AStar_::changeMap(vector<vector<int>> &grid) {
    this->grid.clear();
    std::copy(grid.begin(), grid.end(), back_inserter(this->grid));
}

void AStar_::orderNodeLinks(node_ root) {
    int order = 1;
    if (isDestinationFound(root)) {
        return;
    }
    auto current_first = childParent.find(root)->first;
    current_first.order = order++;
    while(!isDestinationFound(current_first)) {
        auto current_second = childParent.find(current_first)->second;
        current_second.order = order++;
        childParent.erase(current_first);
        childParent.insert(make_pair(current_first, current_second));
        current_first = current_second;
    }
}

/// Both nodes must be on path (excludes destination)
int AStar_::compareNodeOrders(node_ first, node_ second) {
    if (childParent.find(first) == childParent.end() or childParent.find(second) == childParent.end()) {
        logger->logInfo("Nodes to compare are absent or marked as destination")->endLineInfo();
        return -100;
    }
    return childParent.find(first)->first.order - childParent.find(second)->first.order;
}

void AStar_::eraseDestinationNode() {
    node_ dest(destination.first, destination.second);
    if(childParent.find(dest) != childParent.end()) {
        childParent.erase(dest);
    }
}

int AStar_::getNodeOrder(node_ n) {
    auto n_iter = childParent.find(n);
    if(n_iter != childParent.end()) {
        return n_iter->first.order;
    }
    logger->logInfo("Error: Node NOT FOUND for node order")->endLineInfo();
    return -1;
}

int AStar_::getTotalDistanceToDestination() {
    return countOfNodesToDestination;
}
