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

    // fill with static obstacles
    FixedObstacles fixedObstacles;
    for(int obstacle=0; obstacle<TOTAL_FIXED_OBSTACLES; obstacle++) {
        int x_s = obstacles[obstacle][0];
        int x_e = obstacles[obstacle][1];
        int y_s = obstacles[obstacle][2];
        int y_e = obstacles[obstacle][3];
        fixedObstacles.createBlockObstacle(x_s, x_e, y_s, y_e, grid);
    }
}

void GameEnv::createMap2() {
    createEmptyGrid();
    fixedEnemy f1(GRID_SPAN/2, GRID_SPAN/2, 1);
    enemies.push_back(f1);
}
/*
void GameEnv::startGame(int mapNo) {
    createMap2();
    gameSimulation game;
    game.learnToPlay(grid, enemies);
}
*/
void GameEnv::train(int mapNo) {
    createMap2();
    player player1(0, 0, 10, GRID_SPAN -1, GRID_SPAN -1);
    player1.learnGame(grid, enemies);
}

