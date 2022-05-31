//
// Created by Debraj Ray on 2022-01-07.
//

#ifndef EXAMPLE_OBJECTLOCATOR_H
#define EXAMPLE_OBJECTLOCATOR_H

#include <map>

using namespace std;

class objectLocator {

    double l1[9];
    double l2[9];
    double p1[9];
    double p2[9];

    double object_l;
    double object_p;
    double object_distance;
    double object_angle;
    int quadrant;
    double risk_distance;

    double risk_feature;

    void init();

    double calculateDeterminant(int direction);

    void computeDistance();

    void computeCosine();

    void computeSine();

    void findQuadrant();

    void measureUniqueAngle(); // psi

    void calculateRiskFromDistance();

    void calculateRiskFeatures();



    double round_values(double value);

public:

    objectLocator() {
        init();
    }

    void locateObject(int player_x, int player_y, int direction, int object_x, int object_y);

    double getObjectDistance();

    double getObjectAngle();

    double getObjectRiskFeature();
};


#endif //EXAMPLE_OBJECTLOCATOR_H
