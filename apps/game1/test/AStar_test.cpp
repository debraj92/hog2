//
// Created by Debraj Ray on 2022-04-29.
//

#include <gtest/gtest.h>
#include <vector>
#include "../AStar_.h"
#include "../gameConstants.h"

using namespace std;

// Diagonal path
TEST(AStarFindPathDiagonal, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
    AStar_ aStar(grid, 0, 0, 11, 11);
    bool found = aStar.findPathToDestination();
    ASSERT_TRUE(found);
}

TEST(AStarFindPathVertical, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
    AStar_ aStar(grid, 0, 0, 0, 6);
    bool found = aStar.findPathToDestination();
    ASSERT_TRUE(found);
}

TEST(AStarFindPathNE, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
    AStar_ aStar(grid, 5, 6, 11, 0);
    bool found = aStar.findPathToDestination();
    ASSERT_TRUE(found);
}