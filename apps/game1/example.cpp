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

void runTest(player &player1, vector<vector<int>> &grid, std::vector<enemy> &enemies) {
    TestResult t{};
    int sx=0;
    int sy=0;
    int dx=GRID_SPAN-1;
    int dy=GRID_SPAN-1;

    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    assert(t.final_x == dx and t.final_y == dy);


    sx=GRID_SPAN-1;
    sy=GRID_SPAN-1;
    dx=0;
    dy=(GRID_SPAN-1)/2;
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    assert(t.final_x == dx and t.final_y == dy);


    sx=0;
    sy=(GRID_SPAN-1)/2;
    dx=GRID_SPAN-1;
    dy=0;
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    assert(t.final_x == dx and t.final_y == dy);


    sx=0;
    sy=(GRID_SPAN-1)/2;
    dx=GRID_SPAN-1;
    dy=0;
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    assert(t.final_x == dx and t.final_y == dy);

    sx=(GRID_SPAN)/2 + 1;
    sy=GRID_SPAN-1;
    dx=GRID_SPAN-1;
    dy=(GRID_SPAN-1)/2;
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    assert(t.final_x == dx and t.final_y == dy);

    sx=GRID_SPAN-4;
    sy=0;
    dx=0;
    dy=GRID_SPAN - 1;
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    assert(t.final_x == dx and t.final_y == dy);

    sx=GRID_SPAN-1;
    sy=GRID_SPAN-4;
    dx=0;
    dy=GRID_SPAN-3;
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    assert(t.final_x == dx and t.final_y == dy);


    sx=0;
    sy=5;
    dx=GRID_SPAN-1;
    dy=2;
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    assert(t.final_x == dx and t.final_y == dy);



    sx=(GRID_SPAN-1)/2;
    sy=0;
    dx=GRID_SPAN/2 + 3;
    dy=GRID_SPAN - 1;
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    assert(t.final_x == dx and t.final_y == dy);

    sx=9;
    sy=9;
    dx=4;
    dy=0;

    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    assert(t.final_x == dx and t.final_y == dy);
}

int main() {

    increaseStackSize();

    using namespace RTS;
    Logger::GLOBAL_LOG_LEVEL = LOG_LEVEL::INFO;

    //player player1(true);
    //player1.loadExistingModel();
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
    runTest(player1, grid, enemies);


}
