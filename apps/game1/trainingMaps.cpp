//
// Created by Debraj Ray on 2022-06-15.
//

#include "trainingMaps.h"
#include "fixedobstacles.h"
#include "gameConstants.h"
#include <testing.h>

#include <utility>
#include <random>
#include "JsonParser.h"

using namespace std;

///////////////// Game Maps for training //////////////////////////////////////////

void trainingMaps::createMap1(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {
    if(not trainingMaps::isMap1Cached) {
        JsonParser jp("map1");
        jp.readFromFileObstacles(grid);
        jp.readFromFileEnemies(grid, enemies);
        /// store in cache
        std::copy(grid.begin(), grid.end(), back_inserter(trainingMaps::map1_cached_grid));
        std::copy(enemies.begin(), enemies.end(), back_inserter(trainingMaps::map1_cached_enemy));
        trainingMaps::isMap1Cached = true;
        return;
    }
    // restore from cache
    std::copy(trainingMaps::map1_cached_enemy.begin(), trainingMaps::map1_cached_enemy.end(), back_inserter(enemies));
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            grid[i][j] = map1_cached_grid[i][j];
        }
    }
}


void trainingMaps::createMap2(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {
    if(not trainingMaps::isMap2Cached) {
        JsonParser jp("map2");
        jp.readFromFileObstacles(grid);
        jp.readFromFileEnemies(grid, enemies);
        /// store in cache
        std::copy(grid.begin(), grid.end(), back_inserter(trainingMaps::map2_cached_grid));
        std::copy(enemies.begin(), enemies.end(), back_inserter(trainingMaps::map2_cached_enemy));
        trainingMaps::isMap2Cached = true;
        return;
    }
    // restore from cache
    std::copy(trainingMaps::map2_cached_enemy.begin(), trainingMaps::map2_cached_enemy.end(), back_inserter(enemies));
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            grid[i][j] = map2_cached_grid[i][j];
        }
    }
}

void trainingMaps::createMap3(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {
    if(not trainingMaps::isMap3Cached) {
        JsonParser jp("map3");
        jp.readFromFileObstacles(grid);
        jp.readFromFileEnemies(grid, enemies);
        /// store in cache
        std::copy(grid.begin(), grid.end(), back_inserter(trainingMaps::map3_cached_grid));
        std::copy(enemies.begin(), enemies.end(), back_inserter(trainingMaps::map3_cached_enemy));
        trainingMaps::isMap3Cached = true;
        return;
    }
    // restore from cache
    std::copy(trainingMaps::map3_cached_enemy.begin(), trainingMaps::map3_cached_enemy.end(), back_inserter(enemies));
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            grid[i][j] = map3_cached_grid[i][j];
        }
    }
}

void trainingMaps::createMap4(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {
    if(not trainingMaps::isMap4Cached) {
        JsonParser jp("map4");
        jp.readFromFileObstacles(grid);
        jp.readFromFileEnemies(grid, enemies);
        /// store in cache
        std::copy(grid.begin(), grid.end(), back_inserter(trainingMaps::map4_cached_grid));
        std::copy(enemies.begin(), enemies.end(), back_inserter(trainingMaps::map4_cached_enemy));
        trainingMaps::isMap4Cached = true;
        return;
    }
    // restore from cache
    std::copy(trainingMaps::map4_cached_enemy.begin(), trainingMaps::map4_cached_enemy.end(), back_inserter(enemies));
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            grid[i][j] = map4_cached_grid[i][j];
        }
    }
}


void trainingMaps::createMap5(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {
    if(not trainingMaps::isMap5Cached) {
        JsonParser jp("map5");
        jp.readFromFileObstacles(grid);
        jp.readFromFileEnemies(grid, enemies);
        /// store in cache
        std::copy(grid.begin(), grid.end(), back_inserter(trainingMaps::map5_cached_grid));
        std::copy(enemies.begin(), enemies.end(), back_inserter(trainingMaps::map5_cached_enemy));
        trainingMaps::isMap5Cached = true;
        return;
    }
    // restore from cache
    std::copy(trainingMaps::map5_cached_enemy.begin(), trainingMaps::map5_cached_enemy.end(), back_inserter(enemies));
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            grid[i][j] = map5_cached_grid[i][j];
        }
    }
}

void trainingMaps::createMap6(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {
    if(not trainingMaps::isMap6Cached) {
        JsonParser jp("map6");
        jp.readFromFileObstacles(grid);
        jp.readFromFileEnemies(grid, enemies);
        /// store in cache
        std::copy(grid.begin(), grid.end(), back_inserter(trainingMaps::map6_cached_grid));
        std::copy(enemies.begin(), enemies.end(), back_inserter(trainingMaps::map6_cached_enemy));
        trainingMaps::isMap6Cached = true;
        return;
    }
    // restore from cache
    std::copy(trainingMaps::map6_cached_enemy.begin(), trainingMaps::map6_cached_enemy.end(), back_inserter(enemies));
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            grid[i][j] = map6_cached_grid[i][j];
        }
    }
}

void trainingMaps::createMap7(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {
    if(not trainingMaps::isMap7Cached) {
        JsonParser jp("map7");
        jp.readFromFileObstacles(grid);
        jp.readFromFileEnemies(grid, enemies);
        /// store in cache
        std::copy(grid.begin(), grid.end(), back_inserter(trainingMaps::map7_cached_grid));
        std::copy(enemies.begin(), enemies.end(), back_inserter(trainingMaps::map7_cached_enemy));
        trainingMaps::isMap7Cached = true;
        return;
    }
    // restore from cache
    std::copy(trainingMaps::map7_cached_enemy.begin(), trainingMaps::map7_cached_enemy.end(), back_inserter(enemies));
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            grid[i][j] = map7_cached_grid[i][j];
        }
    }
}

void trainingMaps::createMap8(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {
    if(not trainingMaps::isMap8Cached) {
        JsonParser jp("map8");
        jp.readFromFileObstacles(grid);
        jp.readFromFileEnemies(grid, enemies);
        /// store in cache
        std::copy(grid.begin(), grid.end(), back_inserter(trainingMaps::map8_cached_grid));
        std::copy(enemies.begin(), enemies.end(), back_inserter(trainingMaps::map8_cached_enemy));
        trainingMaps::isMap8Cached = true;
        return;
    }
    // restore from cache
    std::copy(trainingMaps::map8_cached_enemy.begin(), trainingMaps::map8_cached_enemy.end(), back_inserter(enemies));
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            grid[i][j] = map8_cached_grid[i][j];
        }
    }
}

void trainingMaps::createMap9(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {
    if(not trainingMaps::isMap9Cached) {
        JsonParser jp("map9");
        jp.readFromFileObstacles(grid);
        jp.readFromFileEnemies(grid, enemies);
        /// store in cache
        std::copy(grid.begin(), grid.end(), back_inserter(trainingMaps::map9_cached_grid));
        std::copy(enemies.begin(), enemies.end(), back_inserter(trainingMaps::map9_cached_enemy));
        trainingMaps::isMap9Cached = true;
        return;
    }
    // restore from cache
    std::copy(trainingMaps::map9_cached_enemy.begin(), trainingMaps::map9_cached_enemy.end(), back_inserter(enemies));
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            grid[i][j] = map9_cached_grid[i][j];
        }
    }
}

void trainingMaps::createMap10(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {
    if(not trainingMaps::isMap10Cached) {
        JsonParser jp("map10");
        jp.readFromFileObstacles(grid);
        jp.readFromFileEnemies(grid, enemies);
        /// store in cache
        std::copy(grid.begin(), grid.end(), back_inserter(trainingMaps::map10_cached_grid));
        std::copy(enemies.begin(), enemies.end(), back_inserter(trainingMaps::map10_cached_enemy));
        trainingMaps::isMap10Cached = true;
        return;
    }
    // restore from cache
    std::copy(trainingMaps::map10_cached_enemy.begin(), trainingMaps::map10_cached_enemy.end(), back_inserter(enemies));
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            grid[i][j] = map10_cached_grid[i][j];
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////


///////////////// Game Maps for testing //////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////

trainingMaps::trainingMaps(bool isTesting) {
    logger = std::make_unique<Logger>(LogLevel);
    if (not isTesting) {
        gameMaps.emplace_back(&trainingMaps::createMap1);
        gameMaps.emplace_back(&trainingMaps::createMap2);
        gameMaps.emplace_back(&trainingMaps::createMap3);
        gameMaps.emplace_back(&trainingMaps::createMap4);
        /*
        gameMaps.emplace_back(&trainingMaps::createMap5);
        gameMaps.emplace_back(&trainingMaps::createMap6);
        gameMaps.emplace_back(&trainingMaps::createMap7);
        gameMaps.emplace_back(&trainingMaps::createMap8);
        */

        isMap1Cached = false;
        isMap2Cached = false;
        isMap3Cached = false;
        isMap4Cached = false;
        /*
        isMap5Cached = false;
        isMap6Cached = false;
        isMap7Cached = false;
        isMap8Cached = false;
         */

    } else {
        /*
        gameMaps.emplace_back(&trainingMaps::createMap1);
        gameMaps.emplace_back(&trainingMaps::createMap2);
        gameMaps.emplace_back(&trainingMaps::createMap3);
        gameMaps.emplace_back(&trainingMaps::createMap4);
        */

        gameMaps.emplace_back(&trainingMaps::createMap5);
        gameMaps.emplace_back(&trainingMaps::createMap6);
        gameMaps.emplace_back(&trainingMaps::createMap7);
        gameMaps.emplace_back(&trainingMaps::createMap8);

        isMap1Cached = false;
        isMap2Cached = false;
        isMap3Cached = false;
        isMap4Cached = false;
        isMap5Cached = false;
        isMap6Cached = false;
        isMap7Cached = false;
        isMap8Cached = false;
    }

}

void trainingMaps::generateNextMap(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies) {
    clearMapAndEnemies(grid, enemies);
    (this->*gameMaps[index])(grid, enemies);
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


void trainingMaps::setSourceAndDestination(vector<std::vector<int>> &grid, int &startX, int &startY, int &endX, int &endY) {
    // setting source
    // pick side
    int side_start = selectRandomNumberInRange(1, 4);
    selectRandomEmptyCoordinateAtBorder(grid, side_start, startX, startY);
    logger->logDebug("Start coordinate (")->logDebug(startX)->logDebug(", ")->logDebug(startY)->logDebug(")")->endLineDebug();
    // setting destination
    // pick side = [1, 4] - start_side
    int side_end = side_start < 3 ? side_start + 2: side_start - 2;
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

int trainingMaps::selectRandomNumberInRange(const int MIN, const int MAX, long seed) {
    std::uniform_int_distribution<> distri(MIN, MAX);
    std::default_random_engine re;
    re.seed(seed);
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
    startY = 9;
    endX = 24;
    endY = 12;
}

void trainingMaps::clearMapAndEnemies(vector<std::vector<int>> &grid, std::vector<enemy>& enemies) {
    enemies.clear();
    for(int i=0; i<grid.size(); i++) {
        for(int j=0; j<grid.size(); j++) {
            grid[i][j] = 0;
        }
    }
}

void trainingMaps::serializeRandomMap(string mapId, long randomNumber) {

    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }


    JsonParser jp(std::move(mapId));

    const int TOTAL_FIXED_OBSTACLES = 25;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4];
    bool horizontal = true;
    long seedX = 1 + randomNumber;
    long seedY = 100 + randomNumber;
    for(int i=1; i<=TOTAL_FIXED_OBSTACLES;) {
        // choose random x
        auto x = trainingMaps::selectRandomNumberInRange(2, GRID_SPAN - 3, seedX);
        // choose random y
        auto y = trainingMaps::selectRandomNumberInRange(2, GRID_SPAN - 3, seedY);
        if (horizontal) {
            if(grid[x][y] == 0 and grid[x][y+1] == 0 and areBoundariesOfObstacleClear(grid, x, y, true)) {
                blockObstacles[i-1][0] = x;
                blockObstacles[i-1][1] = x;
                blockObstacles[i-1][2] = y;
                blockObstacles[i-1][3] = y+1;
                i++;
                horizontal = not horizontal;
                grid[x][y] = -1;
                grid[x][y+1] = -1;
            }
        } else if(grid[x][y] == 0 and grid[x+1][y] == 0 and areBoundariesOfObstacleClear(grid, x, y, false)) {
            blockObstacles[i-1][0] = x;
            blockObstacles[i-1][1] = x+1;
            blockObstacles[i-1][2] = y;
            blockObstacles[i-1][3] = y;
            i++;
            horizontal = not horizontal;
            grid[x][y] = -1;
            grid[x+1][y] = -1;
        }
        seedX += 5;
        seedY += 2;
    }
    jp.serializeObstacles(TOTAL_FIXED_OBSTACLES, blockObstacles);

    /// control percentage of enemies by changing calls to placeEnemies
    std::vector<enemy> enemies;
    // place fixed enemies uniformly
    auto lastId = placeEnemies(grid, enemies, 1, randomNumber);
    lastId = placeEnemies(grid, enemies, lastId, randomNumber);
    // place moving enemies uniformly
    lastId = placeEnemies(grid, enemies, 2, randomNumber);
    lastId = placeEnemies(grid, enemies, lastId, randomNumber);

    const int TOTAL_ENEMIES = enemies.size();
    int enemyLocations[TOTAL_ENEMIES][3];
    for(int i=0; i<TOTAL_ENEMIES; i++) {
        enemyLocations[i][0] = enemies[i].current_x;
        enemyLocations[i][1] = enemies[i].current_y;
        enemyLocations[i][2] = enemies[i].isFixed;
    }
    jp.serializeEnemies(TOTAL_ENEMIES, enemyLocations);

    logger->printBoardDebug(grid);
}

int trainingMaps::placeEnemies(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies, int startId, long randomNumber) {
    int current_row_start = 0;
    int current_col_start = 0;
    long seedX;
    long seedY;
    if (startId % 2 == 1) {
        seedX = 5000 + randomNumber + startId;
        seedY = 1000 + randomNumber + startId;
    } else {
        seedX = 1 + randomNumber + startId;
        seedY = 100 + randomNumber + startId;
    }
    for(current_row_start=VISION_RADIUS; current_row_start<GRID_SPAN - FOV_WIDTH; current_row_start+=MAP_SECTOR_SIZE) {
        for(current_col_start=VISION_RADIUS; current_col_start<GRID_SPAN - FOV_WIDTH; current_col_start+=MAP_SECTOR_SIZE) {
            int central_x = current_row_start + FOV_WIDTH;
            int central_y = current_col_start + FOV_WIDTH;
            if(grid[central_x][central_y] == 0) {
                grid[central_x][central_y] = startId;
                auto e = enemy(grid, central_x, central_y, startId, startId % 2 == 1);
                enemies.push_back(e);
                startId += 2;
                startId = startId == PLAYER_ID ? startId + 2: startId;
            } else {
                bool done = false;
                while(not done) {
                    int x_diff, y_diff;
                    x_diff = trainingMaps::selectRandomNumberInRange(-FOV_WIDTH, FOV_WIDTH, seedX);
                    y_diff = trainingMaps::selectRandomNumberInRange(-FOV_WIDTH, FOV_WIDTH, seedY);
                    bool isXOnBorder = (central_x - x_diff) < VISION_RADIUS or (GRID_SPAN - 1 - (central_x - x_diff)) < VISION_RADIUS;
                    bool isYOnBorder = (central_y - y_diff) < VISION_RADIUS or (GRID_SPAN - 1 - (central_y - y_diff)) < VISION_RADIUS;
                    if(not isXOnBorder and not isYOnBorder and grid[central_x - x_diff][central_y - y_diff] == 0) {
                        done = true;
                        grid[central_x - x_diff][central_y - y_diff] = startId;
                        auto e = enemy(grid, central_x - x_diff, central_y - y_diff, startId, startId % 2 == 1);
                        enemies.push_back(e);
                        startId += 2;
                        startId = startId == PLAYER_ID ? startId + 2: startId;
                    }
                    seedX *= 7;
                    seedY *= 23;
                }
            }
        }
    }
    return startId;
}

bool trainingMaps::areBoundariesOfObstacleClear(vector<std::vector<int>> &grid, int x, int y, bool isHorizontal) {
    if (isHorizontal) {
        if(grid[x][y-1] < 0 or grid[x][y+2] < 0) return false;
        for(int c=y-1; c<=y+2; c++) {
            for(int r=x-1; r<=x+1; r+=2) {
                if(grid[r][c] < 0) return false;
            }
        }
    } else {
        if(grid[x-1][y] < 0 or grid[x+2][y] < 0) return false;
        for(int r=x-1; r<=x+2; r++) {
            for(int c=y-1; c<=y+1; c+=2) {
                if(grid[r][c] < 0) return false;
            }
        }
    }
    return true;
}

#ifdef TESTING
void trainingMaps::createMapUnitTesting1(vector<std::vector<int>> &grid, vector<enemy> &enemies) {
    /// 10X10

    const int TOTAL_FIXED_OBSTACLES = 5;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {3, 3, 4, 6},
            {4, 5, 5, 5},
            {3, 5, 8, 9},
            {5, 5, 2, 2},
            {6, 7, 2, 3}

    };

    FixedObstacles fixedObstacles;
    for(int obstacle=0; obstacle<TOTAL_FIXED_OBSTACLES; obstacle++) {
        int x_s = blockObstacles[obstacle][0];
        int x_e = blockObstacles[obstacle][1];
        int y_s = blockObstacles[obstacle][2];
        int y_e = blockObstacles[obstacle][3];
        fixedObstacles.createBlockObstacle(x_s, x_e, y_s, y_e, grid);
    }

    const int TOTAL_FIXED_ENEMIES = 5;
    int enemyLocations[TOTAL_FIXED_ENEMIES][2] = {
            //x, y
            {2, 4},
            {4, 2},
            {5, 3},
            {7, 6},
            {5, 7},

    };
    for(int enemy_id=0; enemy_id < TOTAL_FIXED_ENEMIES; enemy_id++) {
        auto fe = enemy(grid, enemyLocations[enemy_id][0], enemyLocations[enemy_id][1], enemy_id + 1, true);
        enemies.push_back(fe);
    }
}

void trainingMaps::createMapUnitTesting2(vector<std::vector<int>> &grid, vector<enemy> &enemies) {
    /// 10X10

    const int TOTAL_FIXED_OBSTACLES = 3;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {0, 1, 0, 1},
            {4, 4, 2, 3},
            {1, 1, 4, 4}

    };

    FixedObstacles fixedObstacles;
    for(int obstacle=0; obstacle<TOTAL_FIXED_OBSTACLES; obstacle++) {
        int x_s = blockObstacles[obstacle][0];
        int x_e = blockObstacles[obstacle][1];
        int y_s = blockObstacles[obstacle][2];
        int y_e = blockObstacles[obstacle][3];
        fixedObstacles.createBlockObstacle(x_s, x_e, y_s, y_e, grid);
    }
}

void trainingMaps::registerCreateMapFunction(
        const createMapFuncPtr createMapFunc) {
    gameMaps.push_back(createMapFunc);
}

void trainingMaps::unregisterAllCreateMapFunctions() {
    gameMaps.clear();
}
#endif
