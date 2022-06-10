//
// Created by Debraj Ray on 2021-12-30.
//

#include "fixedEnemy.h"

/// Move to next location and clear old location
void enemy::doNextMove() {
    // nothing
}

int enemy::getAttackPoints() {
    return 10;
}

int enemy::getLifeLeft() {
    return life_left;
}

void enemy::takeDamage(int points) {
    life_left -= points;
}