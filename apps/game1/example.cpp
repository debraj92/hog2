#include <torch/torch.h>

#include <iostream>
#include "gameEnv.h"
#include "AStar_.h"
#include "gameConstants.h"
#include "Logger.h"
#include "player.h"
#include "trainingMaps.h"
#include <sys/resource.h>

void increaseStackSize() {
    const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }
}

int main() {

    increaseStackSize();

    using namespace RTS;
    Logger::GLOBAL_LOG_LEVEL = LOG_LEVEL::INFO;

    player player1(true);
    player1.learnGame();


    /*
    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
    std::vector<enemy> enemies;

    trainingMaps::createMap3(grid, enemies);


    player player1(false);
    TestResult t{};
    */


    // ok
    //player1.playGame(grid, enemies, GRID_SPAN-1, GRID_SPAN-1, 0, (GRID_SPAN-1)/2, t);

    //player1.playGame(grid, enemies, 0, 0, GRID_SPAN -1, GRID_SPAN -1, t);
    //player1.playGame(grid, enemies, GRID_SPAN/2+2, 0, GRID_SPAN/2+2, GRID_SPAN-1, t);
    //player1.playGame(grid, enemies, GRID_SPAN/2+2, 0, GRID_SPAN-1, GRID_SPAN-2, t);

}
