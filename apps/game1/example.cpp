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

void runTesting(player &player1) {
    TestResult t{};
    int sx=0;
    int sy=0;
    int dx=0;
    int dy=0;
    trainingMaps tm(true);
    float countDestinationReach = 0;
    float death = 0;
    float max_ep = 3000;
    for (int i=1; i<= max_ep; i++) {
        cout<<"Episode: "<<i<<endl;
        vector<vector<int>> grid;
        for (int i=0; i<GRID_SPAN; i++) {
            std::vector<int> row(GRID_SPAN, 0);
            grid.push_back(row);
        }
        std::vector<enemy> enemies;
        tm.generateNextMap(grid, enemies);

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
    float death_percent = death * 100 / max_ep;
    cout<<"% reach "<<reach_percent<<endl;
    cout<<"% death "<<death_percent<<endl;
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
    player1.enableBaseLinePlayer();
    TestResult t{};
    runTesting(player1);
    */


    /*
    /// GENERATE NEW RANDOM MAP
    trainingMaps tm(true);
    long randomNumber = std::chrono::system_clock::now().time_since_epoch().count();
    tm.serializeRandomMap("map1", randomNumber);
    cout<<endl;
    tm.serializeRandomMap("map2", randomNumber * 7);
    cout<<endl;
    tm.serializeRandomMap("map3", randomNumber * 13);
    cout<<endl;
    tm.serializeRandomMap("map4", randomNumber * 5);
    cout<<endl;
    tm.serializeRandomMap("map5", randomNumber * 3);
    cout<<endl;
    tm.serializeRandomMap("map6", randomNumber * 23);
    cout<<endl;
    tm.serializeRandomMap("map7", randomNumber * 19);
    cout<<endl;
    tm.serializeRandomMap("map8", randomNumber * 17);
    cout<<endl;
    */
    /*
    /// No moving enemies
    tm.serializeRandomMap("map9", randomNumber * 11);
    cout<<endl;
    tm.serializeRandomMap("map10", randomNumber * 29);
     */

}
