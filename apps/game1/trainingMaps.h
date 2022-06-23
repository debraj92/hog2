//
// Created by Debraj Ray on 2022-06-15.
//

#ifndef EXAMPLE_TRAININGMAPS_H
#define EXAMPLE_TRAININGMAPS_H

#include <vector>
#include "enemy/fixedEnemy.h"
#include <functional>
#include <vector>

#include "Logger.h"

using namespace std;

class trainingMaps {

    const LOG_LEVEL LogLevel = LOG_LEVEL::INFO;

    std::unique_ptr<Logger> logger;

    int index = 0;

    int index_src_dest = 0;

    vector<function<void(std::vector<std::vector<int>>&, std::vector<enemy>&)>> gameMaps;

    static void createAllFixedObstacles(std::vector<std::vector<int>>& grid, int TOTAL_FIXED_OBSTACLES, int blockObstacles[][4]);
    static void createFixedEnemiesAtLocations(std::vector<enemy>& enemies, int TOTAL_ENEMIES, int enemyLocations[][2]);
    int selectRandomNumberInRange(int MIN, int MAX);

    void selectRandomEmptyCoordinateAtBorder(vector<std::vector<int>> &grid, int &side, int &X, int &Y);
    void clearMapAndEnemies(vector<std::vector<int>> &grid, std::vector<enemy>& enemies);

public:

    trainingMaps();

    void generateNextMap(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);
    void setSourceAndDestination(vector<std::vector<int>> &grid, int &startX, int &startY, int &endX, int &endY);

    void setSourceAndDestinationRotating(int &startX, int &startY, int &endX, int &endY);
    void setSourceAndDestinationFixed(int &startX, int &startY, int &endX, int &endY);

    static void createMap1(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);
    static void createMap2(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);
    static void createMap3(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);
    static void createMapSimple(std::vector<std::vector<int>>& grid, std::vector<enemy>& enemies);


    /// For testing
    void registerCreateMapFunction(const function<void(std::vector<std::vector<int>>&, std::vector<enemy>&)>& createMapFunc);
    void unregisterAllCreateMapFunctions();

    int testSeed = 1;
};


#endif //EXAMPLE_TRAININGMAPS_H
