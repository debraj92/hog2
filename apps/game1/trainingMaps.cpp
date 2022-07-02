//
// Created by Debraj Ray on 2022-06-15.
//

#include "trainingMaps.h"
#include "fixedobstacles.h"
#include "enemy/fixedEnemy.h"
#include "gameConstants.h"
#include <testing.h>

#include <utility>
#include <random>

using namespace std;

///////////////// Game Maps for training //////////////////////////////////////////

void trainingMaps::createMapSimple(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {

}

void trainingMaps::createMap3(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {

    /// 8X8

    const int TOTAL_FIXED_OBSTACLES = 5;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {3, 3, 4, 6},
            {4, 5, 5, 5},
            {3, 5, 8, 9},
            {5, 5, 2, 2},
            {6, 7, 2, 3}

    };
    createAllFixedObstacles(grid, TOTAL_FIXED_OBSTACLES, blockObstacles);

    const int TOTAL_FIXED_ENEMIES = 5;
    int enemyLocations[TOTAL_FIXED_ENEMIES][2] = {
            //x, y
            {2, 4},
            {4, 2},
            {5, 3},
            {7, 6},
            {5, 7},

    };
    createFixedEnemiesAtLocations(enemies, TOTAL_FIXED_ENEMIES, enemyLocations);
}

void trainingMaps::createMap1(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {

    /// Grid 6X6

    const int TOTAL_FIXED_OBSTACLES = 2;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {2, 2, 2, 3},
            {2, 3, 4, 4}

    };
    createAllFixedObstacles(grid, TOTAL_FIXED_OBSTACLES, blockObstacles);

    const int TOTAL_FIXED_ENEMIES = 5;
    int enemyLocations[TOTAL_FIXED_ENEMIES][2] = {
            //x, y
            {3, 2},
            {4, 3},
            {4, 1},
            {2, 1},
            {1, 3}

    };
    createFixedEnemiesAtLocations(enemies, TOTAL_FIXED_ENEMIES, enemyLocations);
}

void trainingMaps::createMap2(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {

    /// Grid 6X6

    const int TOTAL_FIXED_OBSTACLES = 3;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {2, 2, 1, 1},
            {4, 4, 3, 3},
            {2, 3, 2, 3}

    };
    createAllFixedObstacles(grid, TOTAL_FIXED_OBSTACLES, blockObstacles);

    const int TOTAL_FIXED_ENEMIES = 6;
    int enemyLocations[TOTAL_FIXED_ENEMIES][2] = {
            //x, y
            {4, 1},
            {5, 2},
            {3, 4},
            {1, 4},
            {0, 5},
            {0, 3}

    };
    createFixedEnemiesAtLocations(enemies, TOTAL_FIXED_ENEMIES, enemyLocations);
}

/////////////////////////////////////////////////////////////////////////////////////

trainingMaps::trainingMaps() {
    logger = std::make_unique<Logger>(LogLevel);
    //gameMaps.emplace_back(createMap1);
    //gameMaps.emplace_back(createMap2);
    gameMaps.emplace_back(createMap3);
    //gameMaps.emplace_back(createMapSimple);
}

void trainingMaps::generateNextMap(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {
    clearMapAndEnemies(grid, enemies);
    gameMaps[index](grid, enemies);
    index = (index + 1) % gameMaps.size();
    logger->printBoardDebug(grid);
}

void trainingMaps::createAllFixedObstacles(std::vector<std::vector<int>>& grid, const int TOTAL_FIXED_OBSTACLES, int (*blockObstacles)[4]) {
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

void trainingMaps::createFixedEnemiesAtLocations(vector<enemy> &enemies, const int TOTAL_ENEMIES, int (*enemyLocations)[2]) {
    for(int enemy_id=0; enemy_id < TOTAL_ENEMIES; enemy_id++) {
         auto fe = fixedEnemy(enemyLocations[enemy_id][0], enemyLocations[enemy_id][1], enemy_id + 1);
        enemies.push_back(fe);
    }
}

void trainingMaps::setSourceAndDestination(vector<std::vector<int>> &grid, int &startX, int &startY, int &endX, int &endY) {
    // setting source
    // pick side
    int side_start = selectRandomNumberInRange(1, 4);
    selectRandomEmptyCoordinateAtBorder(grid, side_start, startX, startY);
    logger->logDebug("Start coordinate (")->logDebug(startX)->logDebug(", ")->logDebug(startY)->logDebug(")")->endLineDebug();
    // setting destination
    // pick side = [1, 4] - start_side
    int side_end = selectRandomNumberInRange(1, 3);
    side_end = side_end == side_start ? 4 : side_end;
    selectRandomEmptyCoordinateAtBorder(grid, side_end, endX, endY);
    logger->logDebug("End coordinate (")->logDebug(endX)->logDebug(", ")->logDebug(endY)->logDebug(")")->endLineDebug();
}

int trainingMaps::selectRandomNumberInRange(const int MIN, const int MAX) {
    std::uniform_int_distribution<> distri(MIN, MAX);
    std::default_random_engine re;
#ifdef TESTING
    re.seed(testSeed);
#else
    re.seed(std::chrono::system_clock::now().time_since_epoch().count());
#endif
    return distri(re);
}

/// Excludes corners
void trainingMaps::selectRandomEmptyCoordinateAtBorder(vector<std::vector<int>> &grid, int &side, int &X, int &Y) {
    if (side % 2 == 0) {
        Y = (side / 2 - 1) * (grid.size() - 1);
        vector<int> emptyLocationX;
        for(int x_=1; x_<grid.size()-1; x_++) {
            if(grid[x_][Y] == 0) {
                emptyLocationX.push_back(x_);
            }
        }
        // pick x randomly
        int x_pos = selectRandomNumberInRange(0, emptyLocationX.size() - 1);
        X = emptyLocationX[x_pos];
        return;
    }
    X = ((side + 1) / 2 - 1) * (grid.size() - 1);
    vector<int> emptyLocationY;
    for(int y_=1; y_<grid.size()-1; y_++) {
        if(grid[X][y_] == 0) {
            emptyLocationY.push_back(y_);
        }
    }
    // pick y randomly
    int y_pos = selectRandomNumberInRange(0, emptyLocationY.size() - 1);
    Y = emptyLocationY[y_pos];
}

void trainingMaps::registerCreateMapFunction(
        const function<void(std::vector<std::vector<int>> &, std::vector<enemy> &)>& createMapFunc) {
    gameMaps.push_back(createMapFunc);
}

void trainingMaps::unregisterAllCreateMapFunctions() {
    gameMaps.clear();
}

void trainingMaps::setSourceAndDestinationRotating(int &startX, int &startY, int &endX,
                                                   int &endY) {
    int sources[9][2] = {
            {0, 0},
            {GRID_SPAN-1, GRID_SPAN-1},
            {0, (GRID_SPAN-1)/2},
            {GRID_SPAN-1, 0},
            {(GRID_SPAN)/2 + 1, GRID_SPAN-1},
            {(GRID_SPAN-1)/2, 0},
            {GRID_SPAN-4, 0},
            {GRID_SPAN - 1, GRID_SPAN-4},
            {0, 5},
    };
    int destinations[9][2] = {
            {GRID_SPAN-1, GRID_SPAN-1},
            {0, (GRID_SPAN-1)/2},
            {GRID_SPAN-1, 0},
            {(GRID_SPAN)/2 + 1, GRID_SPAN-1},
            {GRID_SPAN-1, (GRID_SPAN-1)/2},
            {GRID_SPAN/2 + 3, GRID_SPAN - 1},
            {0, GRID_SPAN - 1},
            {0, GRID_SPAN - 3},
            {GRID_SPAN-1, 2},
    };

    startX = sources[index_src_dest][0];
    startY = sources[index_src_dest][1];
    endX = destinations[index_src_dest][0];
    endY = destinations[index_src_dest][1];
    index_src_dest = (index_src_dest+1)%9;
}

void trainingMaps::setSourceAndDestinationFixed(int &startX, int &startY, int &endX,
                                                   int &endY) {

    startX = 0;
    startY = 0;
    endX = GRID_SPAN - 1;
    endY = GRID_SPAN - 1;
}

void trainingMaps::clearMapAndEnemies(vector<std::vector<int>> &grid, std::vector<enemy>& enemies) {
    enemies.clear();
    for(int i=0; i<grid.size(); i++) {
        for(int j=0; j<grid.size(); j++) {
            grid[i][j] = 0;
        }
    }
}
