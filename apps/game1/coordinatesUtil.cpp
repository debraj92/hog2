//
// Created by Debraj Ray on 2022-01-04.
//

#include "coordinatesUtil.h"

// TODO: This is incomplete
int coordinatesUtil::setSWCoordinates(int& x, int& y) {
    if (x < GRID_SPAN - 1 && y > 0) {
        x++;
        y--;
        return 0;
    }
    return -1;
}

int coordinatesUtil::setSECoordinates(int& x, int& y) {
    if (x < GRID_SPAN - 1 && y < GRID_SPAN - 1) {
        x++;
        y++;
        return 0;
    }
    return -1;
}

int coordinatesUtil::setSCoordinates(int& x, int& y) {
    if (x < GRID_SPAN - 1) {
        x++;
        return 0;
    }
    return -1;
}

int coordinatesUtil::setNECoordinates(int& x, int& y) {
    if (x > 0 && y < GRID_SPAN - 1) {
        x--;
        y++;
        return 0;
    }
    return -1;
}

int coordinatesUtil::setECoordinates(int& x, int& y) {
    if (y < GRID_SPAN - 1) {
        y++;
        return 0;
    }
    return -1;
}

int coordinatesUtil::setStraightActionCoordinates(int& x, int& y, int direction) {
    switch(direction) {
        case SE:
            return setSECoordinates(x,y);
    }
}

int coordinatesUtil::setDodgeDiagonalLeftActionCoordinates(int &x, int &y, int direction) {
    switch(direction) {
        case SE:
            return setSCoordinates(x,y);
    }
}

int coordinatesUtil::setDodgeLeftActionCoordinates(int &x, int &y, int direction) {
    switch(direction) {
        case SE:
            return setSWCoordinates(x,y);
    }
}

int coordinatesUtil::setDodgeRightActionCoordinates(int &x, int &y, int direction) {
    switch(direction) {
        case SE:
            return setNECoordinates(x,y);
    }
}

int coordinatesUtil::setDodgeDiagonalRightActionCoordinates(int &x, int &y, int direction) {
    switch(direction) {
        case SE:
            return setECoordinates(x,y);
    }
}