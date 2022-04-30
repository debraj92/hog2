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
            std::cout <<"Number of nodes to destination "<<countTotalNodesInOptimalPath(nextNode)<<endl;
            reverseNodeLinks();
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

// TODO: Handle Obstacles
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
            childNodes.emplace_back(nextNode.x-1, nextNode.y-1);
        }
        if (nextNode.y+1 < GRID_SPAN) {
            childNodes.emplace_back(nextNode.x-1, nextNode.y+1);
        }
        childNodes.emplace_back(nextNode.x-1, nextNode.y);
    }
    if(nextNode.x+1 < GRID_SPAN) {
        //j-1, j, j+1 : 3 moves
        if (nextNode.y-1 >= 0) {
            childNodes.emplace_back(nextNode.x+1, nextNode.y-1);
        }
        if (nextNode.y+1 < GRID_SPAN) {
            childNodes.emplace_back(nextNode.x+1, nextNode.y+1);
        }
        childNodes.emplace_back(nextNode.x+1, nextNode.y);
    }
    if (nextNode.y-1 >= 0) {
        // i : 1 move
        childNodes.emplace_back(nextNode.x, nextNode.y-1);
    }
    if (nextNode.y+1 < GRID_SPAN) {
        // i : 1 move
        childNodes.emplace_back(nextNode.x, nextNode.y+1);
    }
}

bool AStar_::isDestinationFound(node_ node) {
    return node.x == destination.first and node.y == destination.second;
}

int AStar_::countTotalNodesInOptimalPath(node_& current) {
    node_ parent = childParent.find(current)->second;
    int count = 0;
    while(parent !=  current) {
        count++;
        current = parent;
        parent = childParent.find(parent)->second;
    }
    return count+1;
}

node_ AStar_::getNextNode(node_& current) {
    return childParent.find(current)->second;
}

bool AStar_::isOnPath(node_& current) {
    return childParent.find(current) != childParent.end();
}

void AStar_::finalizeNodeLinks() {
    auto childParentIterator = childParent.begin();
    while(childParentIterator != childParent.end()) {
        node_ node = childParentIterator->first;
        if(closedList.find(node) == closedList.end()) {
            childParentIterator++;
            childParent.erase(node);
        } else {
            childParentIterator++;
        }
    }
}

void AStar_::reverseNodeLinks() {
    auto childParentIterator = childParent.begin();
    unordered_map<node_, node_, node_::node_Hash> temp_childParent;
    while(childParentIterator != childParent.end()) {
        temp_childParent.insert(make_pair(childParentIterator->second, childParentIterator->first));
        childParentIterator++;
    }
    childParent = temp_childParent;
}


