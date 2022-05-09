//
// Created by Debraj Ray on 2021-11-14.
//

#ifndef EXAMPLE_GAMEENV_H
#define EXAMPLE_GAMEENV_H

#include <vector>
#include "enemy/fixedEnemy.h"
#include "player.h"

class GameEnv {

    std::vector<std::vector<int>> grid;

    player player1;

    std::vector<enemy> enemies;

public:

    void createEmptyGrid();

    void createMap1();

    void createMap2();

    //void startGame(int mapNo);

    void train();

    void test(int source_x, int source_y, int destination_x, int destination_y, std::vector<enemy> &enemies);

    void printMap();

};
#endif //EXAMPLE_GAMEENV_H
