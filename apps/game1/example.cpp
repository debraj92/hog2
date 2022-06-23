#include <torch/torch.h>

#include <iostream>
#include "gameEnv.h"
#include "AStar_.h"
#include "gameConstants.h"
#include "Logger.h"
#include "player.h"
#include "trainingMaps.h"

int main() {
    using namespace RTS;
    Logger::GLOBAL_LOG_LEVEL = LOG_LEVEL::INFO;

    //player player1(true);
    //player1.learnGame();


    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
    std::vector<enemy> enemies;

    trainingMaps::createMap3(grid, enemies);


    player player1(false);
    TestResult t{};
    // OK
    //player1.playGame(grid, enemies, 0, 0, GRID_SPAN -1, GRID_SPAN -1, t);
    //player1.playGame(grid, enemies, 0, GRID_SPAN/2 -1, GRID_SPAN/2 -1, 0, t);
    //player1.playGame(grid, enemies, 2, 0, GRID_SPAN -1, 4, t);
    //player1.playGame(grid, enemies, 7, 2, 0, 5, t);
    //player1.playGame(grid, enemies, 5, 0, 1, 7, t);

    // Fail
    //player1.playGame(grid, enemies, 0, GRID_SPAN -1, GRID_SPAN -1, 0, t);
    // player1.playGame(grid, enemies, 0, GRID_SPAN/2 -1, GRID_SPAN -1, GRID_SPAN/2 -1, t);
    // ONLY DOES ACTION UNAVAILABLE [Need to check if enough actions are actually available. Consider bringing back L and R]
    //player1.playGame(grid, enemies, 5, 0, 5, 7, t);
    //player1.playGame(grid, enemies, 2, 0, 5, 7, t);

}
