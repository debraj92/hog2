//
// Created by Debraj Ray on 2022-06-19.
//

#include <gtest/gtest.h>
#include "../FOV_CNN/CNN_controller.h"
#include "../fixedobstacles.h"
#include "../enemy/fixedEnemy.h"


void createAllFixedObstacles(int TOTAL_FIXED_OBSTACLES, int blockObstacles[][4], vector<std::vector<int>> &grid) {

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

void createGrid1(vector<std::vector<int>> &grid) {
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }

    const int TOTAL_FIXED_OBSTACLES = 4;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {1, 2, 2, 2},
            {3, 3, 1, 1},
            {3, 3, 3, 3},
            {1, 1, 3, 3}

    };

    createAllFixedObstacles(TOTAL_FIXED_OBSTACLES, blockObstacles, grid);

    fixedEnemy f1(2, 1, 1);
    fixedEnemy f2(2, 3, 2);
    fixedEnemy f3(4, 3, 3);
    fixedEnemy f4(1, 4, 4);
    std::vector<enemy> enemies;
    enemies.push_back(f1);
    enemies.push_back(f2);
    enemies.push_back(f3);
    enemies.push_back(f4);

    for(enemy e: enemies) {
        grid[e.start_x][e.start_y] = e.id;
    }
}

void printBoard(float (&arr)[FOV_WIDTH][FOV_WIDTH]) {
    for(int i=0; i<FOV_WIDTH; i++) {
        for(int j=0; j<FOV_WIDTH; j++) {
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }
}


TEST(CNN_MarkPath, BasicAssertions) {
    Logger logger(LOG_LEVEL::DEBUG);
    vector<std::vector<int>> grid;
    CNN_controller cnn(grid);
    createGrid1(grid);
    logger.printBoardDebug(grid);
    cnn.markPath(0, 0, 5, 5);
}

TEST(CNN_FOV_Channel1, BasicAssertions) {
    Logger logger(LOG_LEVEL::DEBUG);
    vector<std::vector<int>> grid;
    CNN_controller cnn(grid);
    createGrid1(grid);
    logger.printBoardDebug(grid);
    cnn.markPath(0, 0, 5, 5);

    float obstaclesFOV[3][FOV_WIDTH][FOV_WIDTH];
    float enemiesFOV[3][FOV_WIDTH][FOV_WIDTH];
    float pathFOV[3][FOV_WIDTH][FOV_WIDTH];
    cout<<"("<<0<<","<<0<<")"<<endl;
    cnn.populateFOVChannels(0, 0, obstaclesFOV[0], enemiesFOV[0], pathFOV[0]);
    cout<<"obstacles FOV: "<<endl;
    printBoard(obstaclesFOV[0]);
    cout<<endl;
    cout<<"enemies FOV: "<<endl;
    printBoard(enemiesFOV[0]);
    cout<<endl;
    cout<<"path FOV: "<<endl;
    printBoard(pathFOV[0]);
    cout<<endl;

    cout<<"("<<4<<","<<4<<")"<<endl;
    cnn.populateFOVChannels(4, 4, obstaclesFOV[1], enemiesFOV[1], pathFOV[1]);
    cout<<"obstacles FOV: "<<endl;
    printBoard(obstaclesFOV[1]);
    cout<<endl;
    cout<<"enemies FOV: "<<endl;
    printBoard(enemiesFOV[1]);
    cout<<endl;
    cout<<"path FOV: "<<endl;
    printBoard(pathFOV[1]);
    cout<<endl;

    cout<<"("<<5<<","<<0<<")"<<endl;
    cnn.populateFOVChannels(5, 0, obstaclesFOV[2], enemiesFOV[2], pathFOV[2]);
    cout<<"obstacles FOV: "<<endl;
    printBoard(obstaclesFOV[2]);
    cout<<endl;
    cout<<"enemies FOV: "<<endl;
    printBoard(enemiesFOV[2]);
    cout<<endl;
    cout<<"path FOV: "<<endl;
    printBoard(pathFOV[2]);
    cout<<endl;

}