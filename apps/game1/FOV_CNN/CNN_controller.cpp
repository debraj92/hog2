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
        int currentX, int currentY,
        float (&obstaclesFOV)[FOV_WIDTH][FOV_WIDTH],
        float (&enemiesFOV)[FOV_WIDTH][FOV_WIDTH],
        float (&pathFOV)[FOV_WIDTH][FOV_WIDTH]
        ) {

    int i=0, j=0;
    float min = 100;
    for(int r=currentX - VISION_RADIUS; r<=currentX + VISION_RADIUS; r++) {
        j=0;
        for(int c=currentY - VISION_RADIUS; c<=currentY + VISION_RADIUS; c++) {
            if(r < 0 or r >= GRID_SPAN or c < 0 or c >= GRID_SPAN) {
                // location unavailable
                obstaclesFOV[i][j] = 1;
                enemiesFOV[i][j] = 0;
                pathFOV[i][j] = 0;
            } else {
                obstaclesFOV[i][j] = grid[r][c] < 0 ? 1 : 0;
                enemiesFOV[i][j] = grid[r][c] > 0 and grid[r][c] != PLAYER_ID ? 1 : 0;
                pathFOV[i][j] = static_cast<float>(gridWithPathMarked[r][c]);
                if (pathFOV[i][j] >= 1 and pathFOV[i][j] < min) {
                    min = pathFOV[i][j];
                }
            }
            j++;
        }
        i++;
    }
    min--;
    // Normalize path
    if (min > 0) {
        for (i = 0; i<FOV_WIDTH; i++) {
            for (j = 0; j<FOV_WIDTH; j++) {
                if (pathFOV[i][j] >= 1) {
                    pathFOV[i][j] -= min;
                }
            }
        }
    }

}
