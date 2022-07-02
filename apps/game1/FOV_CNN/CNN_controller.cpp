//
// Created by Debraj Ray on 2022-06-19.
//

#include "CNN_controller.h"

void CNN_controller::populateFOVChannels (
        int currentX, int currentY, int direction,
        float (&obstaclesFOV)[FOV_WIDTH][FOV_WIDTH],
        float (&enemiesFOV)[FOV_WIDTH][FOV_WIDTH]) {

    if(direction == 0) {
        for(int i=0; i<FOV_WIDTH; i++) {
            for (int j = 0; j < FOV_WIDTH; j++) {
                obstaclesFOV[i][j] = 0;
                enemiesFOV[i][j] = 0;
            }
        }
        return;
    }

    for(int i=0; i<FOV_WIDTH; i++) {
        for(int j=0; j<FOV_WIDTH; j++) {
            auto relativeCoordinates = fovFilters.direction_mapping[direction][i][j];
            int gridX = currentX - VISION_RADIUS + relativeCoordinates.x;
            int gridY = currentY - VISION_RADIUS + relativeCoordinates.y;
            populateFOVChannelsForLocation(gridX, gridY, i, j, obstaclesFOV, enemiesFOV);
        }
    }

}

void
CNN_controller::populateFOVChannelsForLocation(int grid_x, int grid_y, int fov_x, int fov_y,
                                               float (&obstaclesFOV)[FOV_WIDTH][FOV_WIDTH],
                                               float (&enemiesFOV)[FOV_WIDTH][FOV_WIDTH]) {
    if(grid_x < 0 or grid_x >= GRID_SPAN or grid_y < 0 or grid_y >= GRID_SPAN) {
        // location unavailable
        obstaclesFOV[fov_x][fov_y] = 1;
        enemiesFOV[fov_x][fov_y] = 0;
    } else {
        obstaclesFOV[fov_x][fov_y] = grid[grid_x][grid_y] < 0 ? 1 : 0;
        enemiesFOV[fov_x][fov_y] = grid[grid_x][grid_y] > 0 and grid[grid_x][grid_y] != PLAYER_ID ? 1 : 0;
    }
}
