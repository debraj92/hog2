//
// Created by Debraj Ray on 2022-06-19.
//

#ifndef EXAMPLE_CNN_CONTROLLER_H
#define EXAMPLE_CNN_CONTROLLER_H

#include "../gameConstants.h"
#include "../Logger.h"
#include "FOV_Filters.h"
#include <vector>

using namespace std;

class CNN_controller {

    const LOG_LEVEL LogLevel = LOG_LEVEL::INFO;

    std::unique_ptr<Logger> logger;

    vector<vector<int>> gridWithPathMarked;
    vector<vector<int>> &grid;

    FOV_Filters fovFilters;

    void createEmptyGrid(vector<std::vector<int>> &grid);

    void populateFOVChannelsForLocation(int grid_x, int grid_y, int fov_x, int fov_y, float &min,
                                        float (&obstaclesFOV)[FOV_WIDTH][FOV_WIDTH],
                                        float (&enemiesFOV)[FOV_WIDTH][FOV_WIDTH],
                                        float (&pathFOV)[FOV_WIDTH][FOV_WIDTH]);

public:

    CNN_controller(vector<vector<int>> &grid1) : grid(grid1) {
        logger = std::make_unique<Logger>(LogLevel);
        createEmptyGrid(gridWithPathMarked);
        fovFilters.init();
    }

    void markPath(int srcX, int srcY, int destX, int destY);

    void populateFOVChannels(int currentX, int currentY, int direction,
                             float (&obstaclesFOV)[FOV_WIDTH][FOV_WIDTH],
                             float (&enemiesFOV)[FOV_WIDTH][FOV_WIDTH],
                             float (&pathFOV)[FOV_WIDTH][FOV_WIDTH]);

    void clearGrid(vector<std::vector<int>> &grid);
};


#endif //EXAMPLE_CNN_CONTROLLER_H
