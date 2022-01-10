//
// Created by Debraj Ray on 2021-12-30.
//

#ifndef EXAMPLE_FIXEDENEMY_H
#define EXAMPLE_FIXEDENEMY_H

#include "enemy.h"

class fixedEnemy : public enemy {
public:
    fixedEnemy(int start_x, int start_y, int id) {
        this->start_x = start_x;
        this->start_y = start_y;
        this->current_x = start_x;
        this->current_y = start_y;
        this->id = id;
    }
};


#endif //EXAMPLE_FIXEDENEMY_H
