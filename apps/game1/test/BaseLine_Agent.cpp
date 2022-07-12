//
// Created by Debraj Ray on 2022-07-11.
//

#include <gtest/gtest.h>
#include "../gameConstants.h"
#include "../fixedobstacles.h"
#include "../player.h"
#include <vector>

using namespace std;

void baseline_createAllFixedObstacles(int TOTAL_FIXED_OBSTACLES, int blockObstacles[][4], vector<std::vector<int>> &grid) {

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

void AStarChase_createGrid2_baseline(vector<std::vector<int>> &grid) {
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }

    const int TOTAL_FIXED_OBSTACLES = 5;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {1, 2, 2, 2},
            {2, 3, 3, 3},
            {5, 7, 1, 1},
            {7, 7, 1, 3},
            {4, 6, 7, 7}
    };

    baseline_createAllFixedObstacles(TOTAL_FIXED_OBSTACLES, blockObstacles, grid);
}


TEST(Baseline, dodge1) {
    TestResult t{};
    vector<std::vector<int>> grid;
    AStarChase_createGrid2_baseline(grid);
    player player1(false);
    player1.enableBaseLinePlayer();
    int sx = 0;
    int sy = 5;
    int dx = GRID_SPAN - 1;
    int dy = GRID_SPAN - 1;
    enemy e1 = enemy(4, 7, 1, false);
    std::vector<enemy> enemies;
    enemies.push_back(e1);
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    assert(player1.life_left > 0);
}

TEST(Baseline, dodge2) {
    TestResult t{};
    vector<std::vector<int>> grid;
    AStarChase_createGrid2_baseline(grid);
    player player1(false);
    player1.enableBaseLinePlayer();
    int sx = 0;
    int sy = 5;
    int dx = GRID_SPAN - 1;
    int dy = 5;
    enemy e1 = enemy(4, 7, 1, false);
    std::vector<enemy> enemies;
    enemies.push_back(e1);
    player1.playGame(grid, enemies, sx, sy, dx, dy, t);
    assert(player1.life_left > 0);
}