//
// Created by Debraj Ray on 2021-12-30.
//

#ifndef EXAMPLE_ENEMY_H
#define EXAMPLE_ENEMY_H


class enemy {

    int life_left;

public:
    int id;
    int start_x;
    int start_y;

    int current_x;
    int current_y;

    virtual void doNextMove();
    virtual int getAttackPoints();
    virtual int getLifeLeft();
    virtual void takeDamage(int points);

};


#endif //EXAMPLE_ENEMY_H
