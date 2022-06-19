#include <torch/torch.h>

#include <iostream>
#include "gameEnv.h"
#include "AStar_.h"
#include "gameConstants.h"
#include "Logger.h"
#include "player.h"

int main() {
    using namespace RTS;
    Logger::GLOBAL_LOG_LEVEL = LOG_LEVEL::INFO;
    /*
    GameEnv gameEnv(true);
    gameEnv.createMapEasiest();
    gameEnv.printMap();
    gameEnv.train();
     */

    player player1(true);
    player1.learnGame();

}
