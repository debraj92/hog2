//
// Created by Debraj Ray on 2021-11-14.
//

#include "fixedobstacles.h"
#include "gameConstants.h"


void FixedObstacles::createBlockObstacle(int x_start, int x_end, int y_start, int y_end,
                                         std::vector<std::vector<int>> &grid) {
    for(int row = y_start; row <= y_end; row++) {
        for(int col = x_start; col <= x_end; col++) {
            grid[row][col] = 1;
        }
    }
}
