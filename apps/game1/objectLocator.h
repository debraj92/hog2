//
// Created by Debraj Ray on 2022-01-07.
//

#ifndef EXAMPLE_OBJECTLOCATOR_H
#define EXAMPLE_OBJECTLOCATOR_H

#include <map>

using namespace std;

class objectLocator {

    float l1[9];
    float l2[9];
    float p1[9];
    float p2[9];

    float object_l;
    float object_p;
    float object_distance;
    float object_angle;
    int quadrant;
    float risk_distance;

    float risk_feature;

    void init();

    float calculateDeterminant(int direction);

    void computeDistance();

    void computeCosine();

    void computeSine();

    void findQuadrant();

    void measureUniqueAngle(); // psi

    void calculateRiskFromDistance();

    void calculateRiskFeatures();



    static float round_values(float value);

public:

    objectLocator() {
        init();
    }

    void locateObject(int player_x, int player_y, int direction, int object_x, int object_y);

    float getObjectDistance();

    float getObjectAngle();

    float getObjectRiskFeature();
};


#endif //EXAMPLE_OBJECTLOCATOR_H
