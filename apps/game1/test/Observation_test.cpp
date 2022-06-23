//
// Created by Debraj Ray on 2022-05-09.
//

#include <gtest/gtest.h>
#include <vector>
#include <iostream>

#include "../fixedobstacles.h"
#include "../observation.h"

using namespace std;


TEST(ObservationDirectionTrajectory, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
    int current_x = 0, current_y = 0, dest_x = 9, dest_y = 9;
    /**
     * A star requires GRID_SPAN to be set correctly
     */
    auto fp = std::make_shared<findPath>(grid, current_x, current_y, dest_x, dest_y);
    fp->findPathToDestination();

    observation ob;
    ob.playerX = current_x;
    ob.playerY = current_y;
    ob.locateTrajectoryAndDirection(fp);
    assert(ob.direction == SE);
    assert(ob.trajectory == on_track);

    const int TOTAL_FIXED_OBSTACLES = 1;
    int blockObstacles[TOTAL_FIXED_OBSTACLES][4] = {
            //x_s, x_e, y_s, y_e
            {1, 4, 1, 4}

    };

    // fill with static obstacles
    FixedObstacles fixedObstacles;
    for(int obstacle=0; obstacle<TOTAL_FIXED_OBSTACLES; obstacle++) {
        int x_s = blockObstacles[obstacle][0];
        int x_e = blockObstacles[obstacle][1];
        int y_s = blockObstacles[obstacle][2];
        int y_e = blockObstacles[obstacle][3];
        fixedObstacles.createBlockObstacle(x_s, x_e, y_s, y_e, grid);
    }
    fp->changeMap(grid);
    fp->printMap();
    fp->findPathToDestination();

    ob.direction = 0;
    ob.trajectory = 0;
    ob.playerX = 5;
    ob.playerY = 2;
    ob.locateTrajectoryAndDirection(fp);
    ob.locateRelativeTrajectory();
    assert(ob.direction == SE);
    assert(ob.trajectory == one_deviation_S);
    assert(ob.trajectory_right);

    ob.direction = 0;
    ob.trajectory = 0;
    ob.playerX = 5;
    ob.playerY = 3;
    ob.locateTrajectoryAndDirection(fp);
    ob.locateRelativeTrajectory();
    assert(ob.direction == SE);
    assert(ob.trajectory == one_deviation_S);
    assert(ob.trajectory_right);

    ob.direction = 0;
    ob.trajectory = 0;
    ob.playerX = 5;
    ob.playerY = 4;
    ob.locateTrajectoryAndDirection(fp);
    ob.locateRelativeTrajectory();

    assert(ob.direction == SE);
    assert(ob.trajectory == two_deviation_S);
    assert(ob.trajectory_right);

    ob.direction = 0;
    ob.trajectory = 0;
    ob.playerX = 8;
    ob.playerY = 5;
    ob.locateTrajectoryAndDirection(fp);
    ob.locateRelativeTrajectory();

    assert(ob.direction == E);
    assert(ob.trajectory == one_deviation_S);
    assert(ob.trajectory_right);

    ob.direction = 0;
    ob.trajectory = 0;
    ob.playerX = 9;
    ob.playerY = 2;
    ob.locateTrajectoryAndDirection(fp);
    ob.locateRelativeTrajectory();

    assert(ob.direction == SE);
    assert(ob.trajectory == two_deviation_N);
    assert(ob.trajectory_left == 2);

    fp->changeSourceAndDestination(0, 5, 5, 0);
    fp->findPathToDestination();

    ob.direction = 0;
    ob.trajectory = 0;
    ob.playerX = 3;
    ob.playerY = 6;
    ob.locateTrajectoryAndDirection(fp);

    assert(ob.direction == S);
    assert(ob.trajectory == one_deviation_W);

    ob.locateRelativeTrajectory();
    assert(ob.trajectory_right == 1);
    assert(ob.trajectory_left == 0);
    assert(ob.trajectory_front == 0);
    assert(ob.trajectory_on_track == 0);
    assert(ob.trajectory_off_track == 0);

    ob.direction = 0;
    ob.trajectory = 0;
    ob.playerX = 8;
    ob.playerY = 8;
    ob.locateTrajectoryAndDirection(fp);
    ob.locateRelativeTrajectory();

    assert(ob.direction == 0);
    assert(ob.trajectory == 0);
    assert(ob.trajectory_off_track);

}

TEST(ObservationRelativeTrajectory, BasicAssertions) {
    observation ob;
    ob.trajectory = on_track;
    for (int d=1; d<=8; d++) {
        ob.direction = d;
        ob.locateRelativeTrajectory();

        assert(ob.trajectory_on_track == 1);
        assert(ob.trajectory_left == 0);
        assert(ob.trajectory_right == 0);
        assert(ob.trajectory_front == 0);
        assert(ob.trajectory_off_track == 0);
    }

    ob = observation();
    ob.trajectory = 11;
    ob.direction = 5;
    ob.locateRelativeTrajectory();
    assert(ob.trajectory_on_track == 0);
    assert(ob.trajectory_left == 1);
    assert(ob.trajectory_right == 0);
    assert(ob.trajectory_front == 0);
    assert(ob.trajectory_off_track == 0);

    ob = observation();
    ob.trajectory = 13;
    ob.direction = 8;
    ob.locateRelativeTrajectory();

    assert(ob.trajectory_on_track == 0);
    assert(ob.trajectory_left == 0);
    assert(ob.trajectory_right == 1);
    assert(ob.trajectory_front == 0);
    assert(ob.trajectory_off_track == 0);

    ob = observation();
    ob.trajectory = 22;
    ob.direction = 4;
    ob.locateRelativeTrajectory();

    assert(ob.trajectory_on_track == 0);
    assert(ob.trajectory_left == 2);
    assert(ob.trajectory_right == 0);
    assert(ob.trajectory_front == 0);
    assert(ob.trajectory_off_track == 0);

    ob = observation();
    ob.trajectory = 24;
    ob.direction = 5;
    ob.locateRelativeTrajectory();

    assert(ob.trajectory_on_track == 0);
    assert(ob.trajectory_left == 0);
    assert(ob.trajectory_right == 2);
    assert(ob.trajectory_front == 0);
    assert(ob.trajectory_off_track == 0);

    ob = observation();
    ob.trajectory = 24;
    ob.direction = 3;
    ob.locateRelativeTrajectory();

    assert(ob.trajectory_on_track == 0);
    assert(ob.trajectory_left == 0);
    assert(ob.trajectory_right == 0);
    assert(ob.trajectory_front == 2);
    assert(ob.trajectory_off_track == 0);

    ob = observation();
    ob.trajectory = 12;
    ob.direction = 2;
    ob.locateRelativeTrajectory();

    assert(ob.trajectory_on_track == 0);
    assert(ob.trajectory_left == 0);
    assert(ob.trajectory_right == 0);
    assert(ob.trajectory_front == 1);
    assert(ob.trajectory_off_track == 0);

    ob = observation();
    ob.trajectory = 0;
    ob.direction = 0;
    ob.locateRelativeTrajectory();
    assert(ob.trajectory_on_track == 0);
    assert(ob.trajectory_left == 0);
    assert(ob.trajectory_right == 0);
    assert(ob.trajectory_front == 0);
    assert(ob.trajectory_off_track == 1);

}