//
// Created by Debraj Ray on 2021-12-30.
//

#ifndef EXAMPLE_ENEMY_H
#define EXAMPLE_ENEMY_H

#include "../Logger.h"
#include "../findPath.h"


using namespace RTS;
using namespace std;

class enemy {

    LOG_LEVEL LogLevel = LOG_LEVEL::INFO;
    std::shared_ptr<Logger> logger;

    std::shared_ptr<findPath> fp;

    int timeStep = 0;

public:
    int id;

    int start_x;
    int start_y;

    int current_x;
    int current_y;
    // TODO: Remove
    bool isFixed = false;
    int life_left;

    int lastKnownPlayerX = -1;
    int lastKnownPlayerY = -1;

    int max_moves = FOV_WIDTH;

    struct playerInfo {
        int player_x;
        int player_y;
        int player_direction;
    };

    enemy(vector<std::vector<int>> &grid, int start_x, int start_y, int id, bool fixed) {
        logger = std::make_shared<Logger>(LogLevel);
        this->start_x = start_x;
        this->start_y = start_y;
        this->current_x = start_x;
        this->current_y = start_y;
        this->id = id;
        this->isFixed = fixed;
        this->life_left = 10;
        this->max_moves = fixed? 0 : this->max_moves;
        fp = std::make_shared<findPath>(grid);
    }

    void doNextMove(int time, vector<std::vector<int>> &grid, playerInfo pl_info = {});
    int getAttackPoints();
    int getLifeLeft();
    void takeDamage(int points);
    bool isPlayerInSight(int player_x, int player_y);
    bool isPlayerTracked (int time) const;

    void predictNextPlayerLocation(vector<std::vector<int>> &grid, playerInfo &pl_info);

};


#endif //EXAMPLE_ENEMY_H
