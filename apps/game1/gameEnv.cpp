//
// Created by Debraj Ray on 2021-11-14.
//

#include <iostream>
#include "gameEnv.h"
#include "fixedobstacles.h"
#include "gameConstants.h"
#include "vector"
#include "gameSimulation.h"

using namespace std;

void GameEnv::createEmptyGrid() {
    // initialize an empty grid
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
}

void GameEnv::createMap1() {
    createEmptyGrid();
    const int TOTAL_FIXED_OBSTACLES = 2;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {0, 4, 2, 6},
            {6, 7, 3, 5}

    };

    // fill with static obstacles
    FixedObstacles fixedObstacles;
    for(int obstacle=0; obstacle<TOTAL_FIXED_OBSTACLES; obstacle++) {
        int x_s = blockObstacles[obstacle][0];
        int x_e = blockObstacles[obstacle][1];
        int y_s = blockObstacles[obstacle][2];
        int y_e = blockObstacles[obstacle][3];
        fixedObstacles.createBlockObstacle(x_s, x_e, y_s, y_e, grid);
    }
    fixedEnemy f1(7, 7, 1);
    fixedEnemy f2(7, 6, 2);
    fixedEnemy f3(8, 9, 3);
    fixedEnemy f4(5, 7, 4);
    enemies.push_back(f1);
    enemies.push_back(f2);
    enemies.push_back(f3);
    enemies.push_back(f4);
}

void GameEnv::createMap2() {
    createEmptyGrid();
    fixedEnemy f1(GRID_SPAN/2, GRID_SPAN/2, 1);
    fixedEnemy f2(GRID_SPAN/2-1, GRID_SPAN/2-1, 1);
    enemies.push_back(f1);
    //enemies.push_back(f2);
}

void GameEnv::train() {
    player1.learnGame(grid, enemies);
}

void GameEnv::printMap() {
    for (int row=0; row<GRID_SPAN; row++) {
        for (int col=0; col<GRID_SPAN; col++) {
            cout<<grid[row][col]<<" ";
        }
        cout<<"\n";
    }
}

void GameEnv::test(int source_x, int source_y, int destination_x, int destination_y, std::vector<enemy> &enemies) {
    player1.playGame(grid, enemies, source_x, source_y, destination_x, destination_y);
}

