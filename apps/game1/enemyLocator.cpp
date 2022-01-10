//
// Created by Debraj Ray on 2022-01-07.
//

#include "enemyLocator.h"
#include "gameConstants.h"
#include <cmath>

void enemyLocator::init() {
    l1[SE] = 1 / sqrt(2);
    l2[SE] = 1 / sqrt(2);
    p1[SE] = - 1 / sqrt(2);
    p2[SE] = 1 / sqrt(2);
}

void enemyLocator::locateEnemy(int player_x, int player_y, int direction, int enemy_x, int enemy_y) {
    double det = calculateDeterminant(direction);
    int diff1 = enemy_x - player_x;
    int diff2 = enemy_y - player_y;
    enemy_l = (p2[direction] * diff1 - p1[direction] * diff2) / det;
    enemy_p = (-l2[direction] * diff1 + l1[direction] * diff2) / det;
    computeDistance();
    computeCosine();
}

double enemyLocator::calculateDeterminant(int direction) {
    return l1[direction]*p2[direction] - p1[direction]*l2[direction];
}

void enemyLocator::computeDistance() {
    enemy_distance = sqrt(pow(enemy_l, 2) + pow(enemy_p, 2));
}

void enemyLocator::computeCosine() {
    if(enemy_distance != 0) {
        enemy_cosine = enemy_l / enemy_distance;
    }
}

double enemyLocator::getEnemyDistance() {
    return round_values(enemy_distance);
}

double enemyLocator::getEnemyCosine() {
    return round_values(enemy_cosine);
}

double enemyLocator::round_values(double value) {
    // round to 2 decimal places
    return std::round(value * 100) / 100;
}
