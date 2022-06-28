//
// Created by Debraj Ray on 2022-06-19.
//

#include "CNN_controller.h"
#include "../findPath.h"

void CNN_controller::markPath(int srcX, int srcY, int destX, int destY) {
    findPath fp(grid, srcX, srcY, destX, destY);
    bool pathFound = fp.findPathToDestination();
    if (pathFound) {
        clearGrid(gridWithPathMarked);
        fp.markPathInGrid(gridWithPathMarked, srcX, srcY, destX, destY);
        logger->printBoardDebug(gridWithPathMarked);
    } else {
        logger->logInfo("Cannot find path from (")->logInfo(srcX)->logInfo(", ")->logInfo(srcY)
        ->logInfo(") to (")->logInfo(destX)->logInfo(", ")->logInfo(destY)->logInfo(")")->endLineInfo();
    }
}

void CNN_controller::createEmptyGrid(vector<std::vector<int>> &grid) {

    // initialize an empty grid
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
}

void CNN_controller::clearGrid(std::vector<std::vector<int>> &grid) {
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            grid[i][j] = 0;
        }
    }
}

void CNN_controller::populateFOVChannels (
        int currentX, int currentY, int direction,
        float (&obstaclesFOV)[FOV_WIDTH][FOV_WIDTH],
        float (&enemiesFOV)[FOV_WIDTH][FOV_WIDTH],
        float (&pathFOV)[FOV_WIDTH][FOV_WIDTH]) {

    float min = 100;

    for(int i=0; i<FOV_WIDTH; i++) {
        for(int j=0; j<FOV_WIDTH; j++) {
            auto relativeCoordinates = fovFilters.direction_mapping[direction][i][j];
            int gridX = currentX - VISION_RADIUS + relativeCoordinates.x;
            int gridY = currentY - VISION_RADIUS + relativeCoordinates.y;
            populateFOVChannelsForLocation(gridX, gridY, i, j, min, obstaclesFOV, enemiesFOV, pathFOV);
        }
    }

    min--;
    // Normalize path
    if (min > 0) {
        for (int i = 0; i<FOV_WIDTH; i++) {
            for (int j = 0; j<FOV_WIDTH; j++) {
                if (pathFOV[i][j] >= 1) {
                    pathFOV[i][j] -= min;
                }
            }
        }
    }

}

void
CNN_controller::populateFOVChannelsForLocation(int grid_x, int grid_y, int fov_x, int fov_y, float &min,
                                               float (&obstaclesFOV)[FOV_WIDTH][FOV_WIDTH],
                                               float (&enemiesFOV)[FOV_WIDTH][FOV_WIDTH],
                                               float (&pathFOV)[FOV_WIDTH][FOV_WIDTH]) {
    if(grid_x < 0 or grid_x >= GRID_SPAN or grid_y < 0 or grid_y >= GRID_SPAN) {
        // location unavailable
        obstaclesFOV[fov_x][fov_y] = 1;
        enemiesFOV[fov_x][fov_y] = 0;
        pathFOV[fov_x][fov_y] = 0;
    } else {
        obstaclesFOV[fov_x][fov_y] = grid[grid_x][grid_y] < 0 ? 1 : 0;
        enemiesFOV[fov_x][fov_y] = grid[grid_x][grid_y] > 0 and grid[grid_x][grid_y] != PLAYER_ID ? 1 : 0;
        pathFOV[fov_x][fov_y] = static_cast<float>(gridWithPathMarked[grid_x][grid_y]);
        if (pathFOV[fov_x][fov_y] >= 1 and pathFOV[fov_x][fov_y] < min) {
            min = pathFOV[fov_x][fov_y];
        }
    }
}
