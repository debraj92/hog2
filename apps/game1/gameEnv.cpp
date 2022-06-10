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

void GameEnv::createMapEasiest() {
    createEmptyGrid();

    //fixedEnemy f1(GRID_SPAN/2+1, GRID_SPAN/2+1, 1);
    //enemies.push_back(f1);
    //fixedEnemy f2(GRID_SPAN - 1, GRID_SPAN - 2, 2);
    //enemies.push_back(f2);


    const int TOTAL_FIXED_OBSTACLES = 5;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {3, 3, 3, 3},
            {1, 1, 1, 1},
            {3, 3, 1, 1},
            {0, 0, 2, 2},
            {2, 2, 3, 3}

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

void GameEnv::createMap3() {
    createEmptyGrid();
    const int TOTAL_FIXED_OBSTACLES = 3;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {0, 3, 2, 6},
            {5, 6, 3, 5},
            {4, 6, 7, 9}

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
    fixedEnemy f1(6, 2, 1);
    fixedEnemy f2(9, 6, 2);
    fixedEnemy f3(8, 3, 3);
    fixedEnemy f4(9, 8, 4);
    fixedEnemy f5(4, 5, 5);
    fixedEnemy f6(7, 1, 6);
    fixedEnemy f7(2, 8, 7);
    fixedEnemy f8(0, 8, 8);
    enemies.push_back(f1);
    enemies.push_back(f2);
    enemies.push_back(f3);
    enemies.push_back(f4);
    enemies.push_back(f5);
    enemies.push_back(f6);
    enemies.push_back(f7);
    enemies.push_back(f8);
}

void GameEnv::createMap4() {
    createEmptyGrid();
    const int TOTAL_FIXED_OBSTACLES = 2;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {1, 6, 1, 4},
            {2, 6, 7, 8}

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
    fixedEnemy f1(7, 2, 1);
    fixedEnemy f2(7, 4, 2);
    fixedEnemy f3(8, 8, 3);
    fixedEnemy f4(1, 7, 4);
    enemies.push_back(f1);
    enemies.push_back(f2);
    enemies.push_back(f3);
    enemies.push_back(f4);
}

void GameEnv::createMap2() {
    createEmptyGrid();

    const int TOTAL_FIXED_OBSTACLES = 3;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {1, 3, 1, 3},
            {6, 7, 5, 6},
            {6, 8, 1, 3}

    };
    createAllFixedObstacles(TOTAL_FIXED_OBSTACLES, blockObstacles);

    fixedEnemy f1(4, 7, 1);
    fixedEnemy f2(4, 1, 2);
    fixedEnemy f3(5, 3, 3);
    fixedEnemy f4(4, 4, 4);
    fixedEnemy f5(1, 6, 5);
    enemies.push_back(f1);
    enemies.push_back(f2);
    enemies.push_back(f3);
    enemies.push_back(f4);
    enemies.push_back(f5);
}

void GameEnv::train() {
    player1->learnGame(grid, enemies);
}

void GameEnv::printMap() {
    for (int row=0; row<GRID_SPAN; row++) {
        for (int col=0; col<GRID_SPAN; col++) {
            cout<<grid[row][col]<<" ";
        }
        cout<<"\n";
    }
}

TestResult GameEnv::test(int source_x, int source_y, int destination_x, int destination_y, std::vector<enemy> &enemies) {
    TestResult result;
    player1->playGame(grid, enemies, source_x, source_y, destination_x, destination_y, result);
    return result;
}


void GameEnv::createAllFixedObstacles(int TOTAL_FIXED_OBSTACLES, int blockObstacles[][4]) {

    // fill with static obstacles
    FixedObstacles fixedObstacles;
    for(int obstacle=0; obstacle<TOTAL_FIXED_OBSTACLES; obstacle++) {
        int x_s = blockObstacles[obstacle][0];
        int x_e = blockObstacles[obstacle][1];
        int y_s = blockObstacles[obstacle][2];
        int y_e = blockObstacles[obstacle][3];
        fixedObstacles.createBlockObstacle(x_s, x_e, y_s, y_e, grid);
    }
}


