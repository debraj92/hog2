//
// Created by Debraj Ray on 2022-04-29.
//

#include <gtest/gtest.h>
#include <vector>
#include "../AStar_.h"
#include "../gameConstants.h"
#include "../gameEnv.h"

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

TEST(AStarRL_DodgeDemo1, BasicAssertions) {

GameEnv gameEnv;
gameEnv.createMap2();
gameEnv.train();

fixedEnemy f1(GRID_SPAN/2, GRID_SPAN/2, 1);
std::vector<enemy> enemies;
enemies.push_back(f1);
gameEnv.test(0, 0, GRID_SPAN - 1, GRID_SPAN - 1, enemies);
cout<<"****************************"<<endl;
gameEnv.test(0, (GRID_SPAN)/2, GRID_SPAN-1, (GRID_SPAN) / 2, enemies);
cout<<"****************************"<<endl;
gameEnv.test((GRID_SPAN)/3,GRID_SPAN-1, 0, (GRID_SPAN)/3, enemies);
cout<<"****************************"<<endl;
gameEnv.test(1, (GRID_SPAN)/4, GRID_SPAN-1, GRID_SPAN-1, enemies);
cout<<"****************************"<<endl;
gameEnv.test(GRID_SPAN-1, GRID_SPAN-1, 1, 0, enemies);
}