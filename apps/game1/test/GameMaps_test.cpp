//
// Created by Debraj Ray on 2022-04-29.
//

#include <gtest/gtest.h>
#include "../gameEnv.h"
#include "../gameConstants.h"

using namespace std;

// Map1
TEST(GameEnvMap1, BasicAssertions) {

    GameEnv gameEnv;
    gameEnv.createMap1();
    gameEnv.printMap();

}