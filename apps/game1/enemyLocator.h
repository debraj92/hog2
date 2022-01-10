//
// Created by Debraj Ray on 2022-01-07.
//

#ifndef EXAMPLE_ENEMYLOCATOR_H
#define EXAMPLE_ENEMYLOCATOR_H

#include <map>

using namespace std;

class enemyLocator {

    double l1[9];
    double l2[9];
    double p1[9];
    double p2[9];

    double enemy_l;
    double enemy_p;
    double enemy_distance;
    double enemy_cosine;

    void init();

    double calculateDeterminant(int direction);

    void computeDistance();

    void computeCosine();

    double round_values(double value);

public:

    enemyLocator() {
        init();
    }

    void locateEnemy(int player_x, int player_y, int direction, int enemy_x, int enemy_y);

    double getEnemyDistance();

    double getEnemyCosine();
};


#endif //EXAMPLE_ENEMYLOCATOR_H
