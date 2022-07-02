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

    vector<vector<int>> &grid;

    FOV_Filters fovFilters;

    void populateFOVChannelsForLocation(int grid_x, int grid_y, int fov_x, int fov_y,
                                        float (&obstaclesFOV)[FOV_WIDTH][FOV_WIDTH],
                                        float (&enemiesFOV)[FOV_WIDTH][FOV_WIDTH]);

public:

    CNN_controller(vector<vector<int>> &grid1) : grid(grid1) {
        logger = std::make_unique<Logger>(LogLevel);
        fovFilters.init();
    }

    void populateFOVChannels(int currentX, int currentY, int direction,
                             float (&obstaclesFOV)[FOV_WIDTH][FOV_WIDTH],
                             float (&enemiesFOV)[FOV_WIDTH][FOV_WIDTH]);
};


#endif //EXAMPLE_CNN_CONTROLLER_H
