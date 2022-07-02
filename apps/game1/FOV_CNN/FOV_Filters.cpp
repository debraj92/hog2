//
// Created by Debraj Ray on 2022-06-26.
//

#include "FOV_Filters.h"
#include <iostream>

using namespace std;
void FOV_Filters::init() {

    // N filter
    for(int i=0; i<FOV_WIDTH; i++) {
        for(int j=0; j<FOV_WIDTH; j++) {
            direction_mapping[N][i][j] = {i, j};
        }
    }

    // S filter
    int x=0, y=0;
    for(int i=FOV_WIDTH; i>=0; i--) {
        y=0;
        for(int j=FOV_WIDTH; j>=0; j--) {
            direction_mapping[S][x][y] = {i-1, j-1};
            y++;
        }
        x++;
    }

    // E filter
    createFilterE();

    // W filter
    createFilterW();

    createFilterNE();
    createFilterSE();
    createFilterSW();
    createFilterNW();

}



void FOV_Filters::createFilterNE() {
    for(int i=0; i<FOV_WIDTH; i++) {
        for(int j=0; j<FOV_WIDTH; j++) {
            direction_mapping[NE][i][j] = {i, j};
        }
    }
    rotateFilter(direction_mapping[NE], 1);
    rotateFilter(direction_mapping[NE], 2);
    rotateFilter(direction_mapping[NE], 2);
}

void FOV_Filters::createFilterSE() {
    for(int i=0; i<FOV_WIDTH; i++) {
        for(int j=0; j<FOV_WIDTH; j++) {
            direction_mapping[SE][i][j] = {i, j};
        }
    }
    rotateFilter(direction_mapping[SE], 1);
    rotateFilter(direction_mapping[SE], 1);
    rotateFilter(direction_mapping[SE], 1);

    rotateFilter(direction_mapping[SE], 2);
    rotateFilter(direction_mapping[SE], 2);
    rotateFilter(direction_mapping[SE], 2);
    rotateFilter(direction_mapping[SE], 2);
    rotateFilter(direction_mapping[SE], 2);
    rotateFilter(direction_mapping[SE], 2);
}

void FOV_Filters::createFilterSW() {
    for(int i=0; i<FOV_WIDTH; i++) {
        for(int j=0; j<FOV_WIDTH; j++) {
            direction_mapping[SW][i][j] = {i, j};
        }
    }
    rotateFilter(direction_mapping[SW], 1);
    rotateFilter(direction_mapping[SW], 1);
    rotateFilter(direction_mapping[SW], 1);
    rotateFilter(direction_mapping[SW], 1);
    rotateFilter(direction_mapping[SW], 1);

    rotateFilter(direction_mapping[SW], 2);
    rotateFilter(direction_mapping[SW], 2);
    rotateFilter(direction_mapping[SW], 2);
    rotateFilter(direction_mapping[SW], 2);
    rotateFilter(direction_mapping[SW], 2);
    rotateFilter(direction_mapping[SW], 2);
    rotateFilter(direction_mapping[SW], 2);
    rotateFilter(direction_mapping[SW], 2);
    rotateFilter(direction_mapping[SW], 2);
    rotateFilter(direction_mapping[SW], 2);
}

void FOV_Filters::createFilterNW() {
    for(int i=0; i<FOV_WIDTH; i++) {
        for(int j=0; j<FOV_WIDTH; j++) {
            direction_mapping[NW][i][j] = {i, j};
        }
    }
    rotateFilter(direction_mapping[NW], 1);
    rotateFilter(direction_mapping[NW], 1);
    rotateFilter(direction_mapping[NW], 1);
    rotateFilter(direction_mapping[NW], 1);
    rotateFilter(direction_mapping[NW], 1);
    rotateFilter(direction_mapping[NW], 1);
    rotateFilter(direction_mapping[NW], 1);

    rotateFilter(direction_mapping[NW], 2);
    rotateFilter(direction_mapping[NW], 2);
    rotateFilter(direction_mapping[NW], 2);
    rotateFilter(direction_mapping[NW], 2);
    rotateFilter(direction_mapping[NW], 2);
    rotateFilter(direction_mapping[NW], 2);
    rotateFilter(direction_mapping[NW], 2);
    rotateFilter(direction_mapping[NW], 2);
    rotateFilter(direction_mapping[NW], 2);
    rotateFilter(direction_mapping[NW], 2);
    rotateFilter(direction_mapping[NW], 2);
    rotateFilter(direction_mapping[NW], 2);
    rotateFilter(direction_mapping[NW], 2);
    rotateFilter(direction_mapping[NW], 2);
}

void FOV_Filters::createFilterE() {
    for(int i=0; i<FOV_WIDTH; i++) {
        for(int j=0; j<FOV_WIDTH; j++) {
            direction_mapping[E][i][j] = {i, j};
        }
    }
    rotateFilter(direction_mapping[E], 1);
    rotateFilter(direction_mapping[E], 1);
    rotateFilter(direction_mapping[E], 2);
    rotateFilter(direction_mapping[E], 2);
    rotateFilter(direction_mapping[E], 2);
    rotateFilter(direction_mapping[E], 2);
}

void FOV_Filters::createFilterW() {
    for(int i=0; i<FOV_WIDTH; i++) {
        for(int j=0; j<FOV_WIDTH; j++) {
            direction_mapping[W][i][j] = {i, j};
        }
    }
    rotateFilter(direction_mapping[W], 1);
    rotateFilter(direction_mapping[W], 1);
    rotateFilter(direction_mapping[W], 1);
    rotateFilter(direction_mapping[W], 1);
    rotateFilter(direction_mapping[W], 1);
    rotateFilter(direction_mapping[W], 1);

    rotateFilter(direction_mapping[W], 2);
    rotateFilter(direction_mapping[W], 2);
    rotateFilter(direction_mapping[W], 2);
    rotateFilter(direction_mapping[W], 2);
    rotateFilter(direction_mapping[W], 2);
    rotateFilter(direction_mapping[W], 2);
    rotateFilter(direction_mapping[W], 2);
    rotateFilter(direction_mapping[W], 2);
    rotateFilter(direction_mapping[W], 2);
    rotateFilter(direction_mapping[W], 2);
    rotateFilter(direction_mapping[W], 2);
    rotateFilter(direction_mapping[W], 2);
}

void FOV_Filters::rotateFilter (gridLocation (&directionFilter)[FOV_WIDTH][FOV_WIDTH], int ring) {

    int x=VISION_RADIUS-ring, y=VISION_RADIUS-ring;
    int v = ring;
    int i = -v + 1, j = -v;
    x++;
    gridLocation directionFilter_copy[FOV_WIDTH][FOV_WIDTH];
    copy(&directionFilter[0][0], &directionFilter[0][0] + FOV_WIDTH * FOV_WIDTH, &directionFilter_copy[0][0]);
    while (i <= v) {
        directionFilter[x][y] = {directionFilter_copy[x - 1][y].x, directionFilter_copy[x - 1][y].y};
        i++;
        x++;
    }
    i--;
    x--;
    j++;
    y++;
    while (j <= v) {
        directionFilter[x][y] = {directionFilter_copy[x][y-1].x, directionFilter_copy[x][y-1].y};
        j++;
        y++;
    }
    j--;
    y--;
    i--;
    x--;
    while (i >= -v) {
        directionFilter[x][y] = {directionFilter_copy[x + 1][y].x, directionFilter_copy[x + 1][y].y};
        x--;
        i--;
    }
    i++;
    x++;
    j--;
    y--;
    while (j >= -v) {
        directionFilter[x][y] = {directionFilter_copy[x][y+1].x, directionFilter_copy[x][y+1].y};
        y--;
        j--;
    }
}


