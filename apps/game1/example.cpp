#include <iostream>
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

void runTest2(player &player1) {
    TestResult t{};
    int sx=0;
    int sy=0;
    int dx=0;
    int dy=0;
    trainingMaps tm;
    float countDestinationReach = 0;
    float death = 0;
    float max_ep = 2000;
    for (int i=1; i<= max_ep; i++) {
        cout<<"Episode: "<<i<<endl;
        vector<vector<int>> grid;
        for (int i=0; i<GRID_SPAN; i++) {
            std::vector<int> row(GRID_SPAN, 0);
            grid.push_back(row);
        }
        std::vector<enemy> enemies;
        //trainingMaps::createMapLarge1(grid, enemies);
        trainingMaps::createMap3(grid, enemies);

        tm.setSourceAndDestination(grid, sx, sy, dx, dy);
        player1.playGame(grid, enemies, sx, sy, dx, dy, t);
        if(t.final_x == t.destination_x and t.final_y == t.destination_y) {
            countDestinationReach++;
        }
        if(player1.life_left == 0) {
            death++;
        }
    }

    float reach_percent = countDestinationReach * 100 / max_ep;
    float death_percent = countDestinationReach * 100 / max_ep;
    cout<<"% reach "<<reach_percent<<endl;
    cout<<"% death "<<death_percent<<endl;
}


void runTest(player &player1) {
    int i=1;
    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
    std::vector<enemy> enemies;
    trainingMaps tm;

    TestResult t{};
    int sx=0;
    int sy=0;
    int dx=GRID_SPAN-1;
    int dy=GRID_SPAN-1;


    tm.generateNextMap(grid, enemies);
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);

    if (not(t.final_x == dx and t.final_y == dy)){
        cout<<i++<<endl;
    } else {
        i++;
    }

    sx=GRID_SPAN-1;
    sy=GRID_SPAN-1;
    dx=0;
    dy=(GRID_SPAN-1)/2;
    tm.generateNextMap(grid, enemies);
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    if (not(t.final_x == dx and t.final_y == dy)){
        cout<<i++<<endl;
    } else {
        i++;
    }

    sx=0;
    sy=(GRID_SPAN-1)/2;
    dx=GRID_SPAN-1;
    dy=0;
    tm.generateNextMap(grid, enemies);
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    if (not(t.final_x == dx and t.final_y == dy)){
        cout<<i++<<endl;
    } else {
        i++;
    }

    sx=GRID_SPAN-1;
    sy=0;
    dx=(GRID_SPAN)/2 + 1;
    dy=GRID_SPAN-1;
    tm.generateNextMap(grid, enemies);
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    if (not(t.final_x == dx and t.final_y == dy)){
        cout<<i++<<endl;
    } else {
        i++;
    }


    sx=(GRID_SPAN)/2 + 1;
    sy=GRID_SPAN-1;
    dx=GRID_SPAN-1;
    dy=(GRID_SPAN-1)/2;
    tm.generateNextMap(grid, enemies);
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    if (not(t.final_x == dx and t.final_y == dy)){
        cout<<i++<<endl;
    } else {
        i++;
    }

    sx=(GRID_SPAN-1)/2;
    sy=0;
    dx=GRID_SPAN/2 + 3;
    dy=GRID_SPAN - 1;
    tm.generateNextMap(grid, enemies);
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    if (not(t.final_x == dx and t.final_y == dy)){
        cout<<i++<<endl;
    } else {
        i++;
    }


    sx=GRID_SPAN-4;
    sy=0;
    dx=0;
    dy=GRID_SPAN - 1;
    tm.generateNextMap(grid, enemies);
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    if (not(t.final_x == dx and t.final_y == dy)){
        cout<<i++<<endl;
    } else {
        i++;
    }

    sx=GRID_SPAN-1;
    sy=GRID_SPAN-4;
    dx=0;
    dy=GRID_SPAN-3;
    tm.generateNextMap(grid, enemies);
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    if (not(t.final_x == dx and t.final_y == dy)){
        cout<<i++<<endl;
    } else {
        i++;
    }

    sx=0;
    sy=5;
    dx=GRID_SPAN-1;
    dy=2;
    tm.generateNextMap(grid, enemies);
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    if (not(t.final_x == dx and t.final_y == dy)){
        cout<<i++<<endl;
    } else {
        i++;
    }

}


int main() {

    increaseStackSize();

    using namespace RTS;
    Logger::GLOBAL_LOG_LEVEL = LOG_LEVEL::INFO;

    player player1(true);
    player1.learnGame();

    //player player1(false);
    //player1.loadExistingModel();
    //player1.learnGame();

    /*
    player player1(false);
    /// Enable baseline for comparison
    //player1.enableBaseLinePlayer();
    TestResult t{};
    runTest(player1);
    */


}
