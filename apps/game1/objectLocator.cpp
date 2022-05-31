//
// Created by Debraj Ray on 2022-01-07.
//

#include "objectLocator.h"
#include "gameConstants.h"
#include <cmath>
#include <iostream>

using namespace std;


void objectLocator::init() {
    l1[SE] = 1 / sqrt(2);
    l2[SE] = 1 / sqrt(2);
    p1[SE] = - 1 / sqrt(2);
    p2[SE] = 1 / sqrt(2);

    l1[N] = -1;
    l2[N] = 0;
    p1[N] = 0;
    p2[N] = -1;

    l1[NE] = -1 / sqrt(2);
    l2[NE] = 1 / sqrt(2);
    p1[NE] = -1 / sqrt(2);
    p2[NE] = -1 / sqrt(2);

    l1[NW] = -1 / sqrt(2);
    l2[NW] = -1 / sqrt(2);
    p1[NW] = 1 / sqrt(2);
    p2[NW] = -1 / sqrt(2);

    l1[E] = 0;
    l2[E] = 1;
    p1[E] = -1;
    p2[E] = 0;

    l1[W] = 0;
    l2[W] = -1;
    p1[W] = 1;
    p2[W] = 0;

    l1[S] = 1;
    l2[S] = 0;
    p1[S] = 0;
    p2[S] = 1;

    l1[SW] = 1 / sqrt(2);
    l2[SW] = -1 / sqrt(2);
    p1[SW] = 1 / sqrt(2);
    p2[SW] = 1 / sqrt(2);
}

void objectLocator::locateObject(int player_x, int player_y, int direction, int object_x, int object_y) {
    cout<<"Unit direction"<<direction<<endl;
    double det = calculateDeterminant(direction);
    int diff1 = object_x - player_x;
    int diff2 = object_y - player_y;
    object_l = (p2[direction] * diff1 - p1[direction] * diff2) / det;
    object_p = (-l2[direction] * diff1 + l1[direction] * diff2) / det;
    computeDistance();
    findQuadrant();
    measureUniqueAngle();
    calculateRiskFromDistance();
    calculateRiskFeatures();
}

double objectLocator::calculateDeterminant(int direction) {
    return l1[direction]*p2[direction] - p1[direction]*l2[direction];
}

void objectLocator::computeDistance() {
    object_distance = sqrt(pow(object_l, 2) + pow(object_p, 2));
}

void objectLocator::computeCosine() {
    if(object_distance != 0) {
        object_angle = object_l / object_distance;
    }
}

void objectLocator::computeSine() {
    if(object_distance != 0) {
        object_angle = object_p / object_distance;
    }
}

double objectLocator::getObjectDistance() {
    cout<<"getObjectDistance"<<endl;
    return round_values(object_distance);
}

double objectLocator::round_values(double value) {
    // round to 2 decimal places
    return std::round(value * 100) / 100;
}

void objectLocator::findQuadrant() {
    if (object_l >= 0 and object_p >= 0) {
        quadrant = 1;
    } else if (object_p >= 0) {
        quadrant = 2;
    } else if (object_l >= 0) {
        quadrant = 4;
    } else {
        quadrant = 3;
    }
    cout<<"Quadrant"<<quadrant<<endl;
}

void objectLocator::measureUniqueAngle() {
    switch (quadrant) {
        case 1:
            computeCosine();
            object_angle++;
            break;
        case 2:
        case 3:
            computeSine();
            break;
        case 4:
            computeCosine();
            object_angle *= -1;
            object_angle--;
            break;
    }
    cout<<"measureUniqueAngle "<<object_angle<<endl;
}

void objectLocator::calculateRiskFromDistance() {
    risk_distance = RISK_DISTANCE_MAX_MAGNITUDE * exp(-object_distance);
    cout<<"calculateRiskFromDistance "<<risk_distance<<endl;
}

void objectLocator::calculateRiskFeatures() {
    risk_feature = risk_distance * object_angle;
    cout<<"calculateRiskFeatures "<<risk_feature<<endl;
}

double objectLocator::getObjectRiskFeature() {
    cout<<"objectLocator::getObjectRiskFeature"<<endl;
    return risk_feature;
}

double objectLocator::getObjectAngle() {
    cout<<"objectLocator::getObjectAngle"<<endl;
    return object_angle;
}
