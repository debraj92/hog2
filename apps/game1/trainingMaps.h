//
// Created by Debraj Ray on 2022-06-15.
//

#ifndef EXAMPLE_TRAININGMAPS_H
#define EXAMPLE_TRAININGMAPS_H

#include <vector>
#include <functional>
#include <vector>
#include "enemy/enemy.h"
#include "Logger.h"
#include <testing.h>

using namespace std;

class trainingMaps {

    const LOG_LEVEL LogLevel = LOG_LEVEL::INFO;

    std::unique_ptr<Logger> logger;

    int index = 0;

    int index_src_dest = 0;

    typedef void (trainingMaps::*createMapFuncPtr)(std::vector<std::vector<int>>&, std::vector<enemy>&);
    vector<createMapFuncPtr> gameMaps;

    void createAllFixedObstacles(std::vector<std::vector<int>>& grid, int TOTAL_FIXED_OBSTACLES, int blockObstacles[][4]);
    int selectRandomNumberInRange(int MIN, int MAX);
    static int selectRandomNumberInRange(int MIN, int MAX, long seed);

    void selectRandomEmptyCoordinateAtBorder(vector<std::vector<int>> &grid, int &side, int &X, int &Y);
    void clearMapAndEnemies(vector<std::vector<int>> &grid, std::vector<enemy>& enemies);

    int placeEnemies(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies, int startId, long randomNumber);

public:

    trainingMaps(bool isTesting);

    void generateNextMap(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);
    void setSourceAndDestination(vector<std::vector<int>> &grid, int &startX, int &startY, int &endX, int &endY);

    void setSourceAndDestinationRotating(int &startX, int &startY, int &endX, int &endY);
    void setSourceAndDestinationFixed(int &startX, int &startY, int &endX, int &endY);

    void serializeRandomMap(string mapId, long randomNumber);

    /// training maps
    void createMap1(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);
    void createMap2(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);
    void createMap3(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);

    /// testing maps
    void createMap4(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);
    void createMap5(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);
    void createMap6(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);

    bool isMap1Cached;
    vector<std::vector<int>> map1_cached_grid;
    std::vector<enemy> map1_cached_enemy;

    bool isMap2Cached;
    vector<std::vector<int>> map2_cached_grid;
    std::vector<enemy> map2_cached_enemy;

    bool isMap3Cached;
    vector<std::vector<int>> map3_cached_grid;
    std::vector<enemy> map3_cached_enemy;

    bool isMap4Cached;
    vector<std::vector<int>> map4_cached_grid;
    std::vector<enemy> map4_cached_enemy;

    bool isMap5Cached;
    vector<std::vector<int>> map5_cached_grid;
    std::vector<enemy> map5_cached_enemy;

    bool isMap6Cached;
    vector<std::vector<int>> map6_cached_grid;
    std::vector<enemy> map6_cached_enemy;


#ifdef TESTING
    /// For Unit testing
    void createMapUnitTesting1(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);
    void createMapUnitTesting2(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);
    void registerCreateMapFunction(const createMapFuncPtr createMapFunc);
    void unregisterAllCreateMapFunctions();

    int testSeed = 1;
#endif
};


#endif //EXAMPLE_TRAININGMAPS_H
