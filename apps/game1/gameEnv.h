//
// Created by Debraj Ray on 2021-11-14.
//

#ifndef EXAMPLE_GAMEENV_H
#define EXAMPLE_GAMEENV_H

#include <vector>
#include "enemy/fixedEnemy.h"
#include "player.h"
#include "TestResult.h"

class GameEnv {

    player player1;

    void createAllFixedObstacles(int TOTAL_FIXED_OBSTACLES, int blockObstacles[][4]);

public:

    std::vector<std::vector<int>> grid;

    std::vector<enemy> enemies;

    void createEmptyGrid();

    void createMap1();

    void createMap2();

    void createMap3();

    void createMap4();

    //void startGame(int mapNo);

    void train();

    TestResult test(int source_x, int source_y, int destination_x, int destination_y, std::vector<enemy> &enemies);

    void printMap();

};
#endif //EXAMPLE_GAMEENV_H
