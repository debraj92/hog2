//
// Created by Debraj Ray on 2022-04-29.
//

#include <gtest/gtest.h>
#include <vector>
#include "../AStar_.h"
#include "../gameConstants.h"
#include "../gameEnv.h"
#include "../fixedobstacles.h"

using namespace std;

// Diagonal path
TEST(AStarFindPathDiagonal, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
    AStar_ aStar(grid, 0, 0, GRID_SPAN - 1, GRID_SPAN - 1);
    bool found = aStar.findPathToDestination();
    ASSERT_TRUE(found);
}

TEST(AStarFindPathMultipleEnemies, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }

    const int TOTAL_FIXED_OBSTACLES = 1;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {0, 3, 1, 3}

    };

    // fill with static obstacles
    FixedObstacles fixedObstacles;
    for(int obstacle=0; obstacle<TOTAL_FIXED_OBSTACLES; obstacle++) {
        int x_s = blockObstacles[obstacle][0];
        int x_e = blockObstacles[obstacle][1];
        int y_s = blockObstacles[obstacle][2];
        int y_e = blockObstacles[obstacle][3];
        fixedObstacles.createBlockObstacle(x_s, x_e, y_s, y_e, grid);
    }

    fixedEnemy f1(5, 4, 1);
    fixedEnemy f2(4, 1, 2);
    fixedEnemy f3(5, 3, 3);
    fixedEnemy f4(4, 4, 4);
    vector<enemy> enemies;
    enemies.push_back(f1);
    enemies.push_back(f2);
    enemies.push_back(f3);
    enemies.push_back(f4);

    AStar_ aStar(grid, 5, 2, 5, 5);
    aStar.populateEnemyObstacles(enemies);
    aStar.findPathToDestination();

}

TEST(AStarFindPathVertical, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
    AStar_ aStar(grid, 0, 0, 0, GRID_SPAN - 1);
    bool found = aStar.findPathToDestination();
    ASSERT_TRUE(found);
}

TEST(AStarFindPathNE, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
    AStar_ aStar(grid, 5, 6, GRID_SPAN - 1, 0);
    bool found = aStar.findPathToDestination();
    ASSERT_TRUE(found);
}

TEST(AStarFindPathOrdering, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }

    const int TOTAL_FIXED_OBSTACLES = 2;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {1, 2, 2, 3},
            {4, 5, 2, 3}
    };

    // fill with static obstacles
    FixedObstacles fixedObstacles;
    for(int obstacle=0; obstacle<TOTAL_FIXED_OBSTACLES; obstacle++) {
        int x_s = blockObstacles[obstacle][0];
        int x_e = blockObstacles[obstacle][1];
        int y_s = blockObstacles[obstacle][2];
        int y_e = blockObstacles[obstacle][3];
        fixedObstacles.createBlockObstacle(x_s, x_e, y_s, y_e, grid);
    }
    AStar_ aStar(grid, 1, 4, 5, 1);
    bool found = aStar.findPathToDestination();
    node_ n1(2, 4);
    node_ n2(3, 2);
    assert(aStar.compareNodeOrders(n1, n2) < 0);
    assert(aStar.compareNodeOrders(n2, n1) > 0);

    node_ n3(3, 2);
    assert(aStar.compareNodeOrders(n2, n3) == 0);

    node_ n4(1, 4);
    assert(aStar.compareNodeOrders(n4, n3) < 0);
}

TEST(AStarFindPathOrdering2, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }

    const int TOTAL_FIXED_OBSTACLES = 2;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {1, 2, 2, 3},
            {4, 5, 2, 3}
    };

// fill with static obstacles
    FixedObstacles fixedObstacles;
    for(int obstacle=0; obstacle<TOTAL_FIXED_OBSTACLES; obstacle++) {
        int x_s = blockObstacles[obstacle][0];
        int x_e = blockObstacles[obstacle][1];
        int y_s = blockObstacles[obstacle][2];
        int y_e = blockObstacles[obstacle][3];
        fixedObstacles.createBlockObstacle(x_s, x_e, y_s, y_e, grid);
    }
    findPath fp(grid, 1, 4, 5, 1);
    fp.findPathToDestination();

    assert(fp.isOnTrack(3,3));
    assert(not fp.isOnTrack(1,4));
    assert(fp.isOnTrack(4,1));
    assert(not fp.isOnTrack(5,1));
}

TEST(AStarFindDeleteDestinationNode, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<5; i++) {
        std::vector<int> row(5, 0);
        grid.push_back(row);
    }

    const int TOTAL_FIXED_OBSTACLES = 2;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {1, 3, 2, 2},
            {3, 3, 3, 3}
    };

// fill with static obstacles
    FixedObstacles fixedObstacles;
    for(int obstacle=0; obstacle<TOTAL_FIXED_OBSTACLES; obstacle++) {
        int x_s = blockObstacles[obstacle][0];
        int x_e = blockObstacles[obstacle][1];
        int y_s = blockObstacles[obstacle][2];
        int y_e = blockObstacles[obstacle][3];
        fixedObstacles.createBlockObstacle(x_s, x_e, y_s, y_e, grid);
    }

    AStar_ aStar(grid, 2, 3, 4, 1);
    aStar.findPathToDestination();
    node_ d(4, 1);
    assert(not aStar.isOnPath(d));

    aStar.changeSourceAndDestination(1, 0, 4, 1);
    aStar.findPathToDestination();
    assert(not aStar.isOnPath(d));

    aStar.changeSourceAndDestination(2, 1, 4, 1);
    aStar.findPathToDestination();
    assert(not aStar.isOnPath(d));

    aStar.changeSourceAndDestination(4, 3, 4, 1);
    aStar.findPathToDestination();
    assert(not aStar.isOnPath(d));

    aStar.changeSourceAndDestination(4, 0, 4, 1);
    aStar.findPathToDestination();
    assert(not aStar.isOnPath(d));

    aStar.changeSourceAndDestination(3, 0, 4, 1);
    aStar.findPathToDestination();
    assert(not aStar.isOnPath(d));
}

TEST(AStarNodeOrder, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<5; i++) {
        std::vector<int> row(5, 0);
        grid.push_back(row);
    }

    const int TOTAL_FIXED_OBSTACLES = 2;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {1, 3, 2, 2},
            {3, 3, 3, 3}
    };

// fill with static obstacles
    FixedObstacles fixedObstacles;
    for(int obstacle=0; obstacle<TOTAL_FIXED_OBSTACLES; obstacle++) {
        int x_s = blockObstacles[obstacle][0];
        int x_e = blockObstacles[obstacle][1];
        int y_s = blockObstacles[obstacle][2];
        int y_e = blockObstacles[obstacle][3];
        fixedObstacles.createBlockObstacle(x_s, x_e, y_s, y_e, grid);
    }

    AStar_ aStar(grid, 2, 3, 4, 1);
    aStar.findPathToDestination();

    node_ n(4, 3);
    assert(aStar.getNodeOrder(n) == 3);

    node_ n1(4, 4);
    assert(aStar.getNodeOrder(n1) == -1);
}