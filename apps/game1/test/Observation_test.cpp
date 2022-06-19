//
// Created by Debraj Ray on 2022-05-09.
//

#include <gtest/gtest.h>
#include <vector>
#include <iostream>

#include "../fixedobstacles.h"
#include "../observation.h"

using namespace std;

// Diagonal path
TEST(ObservationRedirect, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<10; i++) {
        std::vector<int> row(10, 0);
        grid.push_back(row);
    }

    observation ob;
    ob.redirect(5, 2, 5, 7);
    assert(ob.direction == E);

    ob.redirect(5, 2, 1, 2);
    assert(ob.direction == N);

    ob.redirect(5, 5, 6, 3);
    assert(ob.direction == SW);

    ob.redirect(5, 5, 6, 0);
    assert(ob.direction == W);

    ob.redirect(5, 5, 2, 3);
    assert(ob.direction == NW);

    ob.redirect(5, 5, 0, 4);
    assert(ob.direction == N);

}

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
    ob.locateTrajectoryAndDirection(fp, dest_x, dest_y);
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
    ob.locateTrajectoryAndDirection(fp, dest_x, dest_y);
    assert(ob.direction == SE);
    assert(ob.trajectory == one_deviation_S);

    ob.direction = 0;
    ob.trajectory = 0;
    ob.playerX = 5;
    ob.playerY = 3;
    ob.locateTrajectoryAndDirection(fp, dest_x, dest_y);
    assert(ob.direction == SE);
    assert(ob.trajectory == one_deviation_S);

    ob.direction = 0;
    ob.trajectory = 0;
    ob.playerX = 5;
    ob.playerY = 4;
    ob.locateTrajectoryAndDirection(fp, dest_x, dest_y);

    assert(ob.direction == SE);
    assert(ob.trajectory == two_deviation_S);

    ob.direction = 0;
    ob.trajectory = 0;
    ob.playerX = 8;
    ob.playerY = 5;
    ob.locateTrajectoryAndDirection(fp, dest_x, dest_y);

    assert(ob.direction == E);
    assert(ob.trajectory == one_deviation_S);

    ob.direction = 0;
    ob.trajectory = 0;
    ob.playerX = 9;
    ob.playerY = 2;
    ob.locateTrajectoryAndDirection(fp, dest_x, dest_y);

    assert(ob.direction == SE);
    assert(ob.trajectory == two_deviation_N);

    fp->changeSourceAndDestination(0, 5, 5, 0);
    fp->findPathToDestination();

    ob.direction = 0;
    ob.trajectory = 0;
    ob.playerX = 3;
    ob.playerY = 6;
    ob.locateTrajectoryAndDirection(fp, 5, 0);

    assert(ob.direction == SW);
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
    ob.locateTrajectoryAndDirection(fp, 5, 0);

    assert(ob.direction == 0);
    assert(ob.trajectory == 0);

}

TEST(ObservationDirectionTrajectory2, BasicAssertions) {
/*
    vector<vector<int>> grid;
    for (int i=0; i<2; i++) {
        std::vector<int> row(2, 0);
        grid.push_back(row);
    }
    int current_x = 0, current_y = 0, dest_x = 1, dest_y = 1;
    observation ob;

    auto fp = std::make_shared<findPath>(grid, current_x, current_y, dest_x, dest_y);
    fp->findPathToDestination();
    ob.direction = 0;
    ob.trajectory = 0;
    ob.playerX = 0;
    ob.playerY = 0;
    ob.locateTrajectoryAndDirection(fp, dest_x, dest_y);
    ob.updateObstacleDistances(grid);

    assert(ob.direction == SE);
    assert(ob.trajectory == on_track);

    float observation_vector[MAX_ABSTRACT_OBSERVATIONS] = {0};
    ob.flattenObservationToVector(observation_vector);
    float expected_result[MAX_ABSTRACT_OBSERVATIONS] = {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 2};
    for(int i=0; i<MAX_ABSTRACT_OBSERVATIONS; i++) {
        assert(expected_result[i] == observation_vector[i]);
    }

    ob.playerX = 1;
    ob.playerY = 0;
    ob.direction = 0;
    ob.trajectory = 0;
    ob.locateTrajectoryAndDirection(fp, dest_x, dest_y);
    ob.updateObstacleDistances(grid);

    assert(ob.direction == SE);
    assert(ob.trajectory == one_deviation_N);

    fill_n(observation_vector, MAX_ABSTRACT_OBSERVATIONS, 0);
    ob.flattenObservationToVector(observation_vector);
    float expected_result2[MAX_ABSTRACT_OBSERVATIONS] = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 2, 1 };
    for(int i=0; i<MAX_ABSTRACT_OBSERVATIONS; i++) {
        assert(expected_result2[i] == observation_vector[i]);
        //cout<<observation_vector[i]<<" ";
    }

    ob.playerX = 0;
    ob.playerY = 1;
    ob.direction = 0;
    ob.trajectory = 0;
    ob.locateTrajectoryAndDirection(fp, dest_x, dest_y);
    ob.updateObstacleDistances(grid);

    assert(ob.direction == SE);
    assert(ob.trajectory == one_deviation_W);

    fill_n(observation_vector, MAX_ABSTRACT_OBSERVATIONS, 0);
    ob.flattenObservationToVector(observation_vector);
    float expected_result3[MAX_ABSTRACT_OBSERVATIONS] = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 2, 1, 2 };
    for(int i=0; i<MAX_ABSTRACT_OBSERVATIONS; i++) {
        assert(expected_result3[i] == observation_vector[i]);
    }
    */
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