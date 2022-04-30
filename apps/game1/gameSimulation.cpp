//
// Created by Debraj Ray on 2021-12-30.
//

#include "gameSimulation.h"

void gameSimulation::learnToPlay(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies) {
    cout<<"gameSimulation::learnToPlay"<<endl;
    populateEnemies(grid, enemies);
    player1->findPathToDestination(grid, player1->current_x, player1->current_y, player1->destination_x, player1->destination_y);
    grid[player1->current_x][player1->current_y] = 19;
    player1->ontrack = true;
    observation ob = player1->createObservation(grid, enemies);
    player1->createStartState(ob);
    player1->cur_state->x = player1->current_x;
    player1->cur_state->y = player1->current_y;
    //player1->cur_state->updateObstacleDistances(grid, player1->current_x, player1->current_y);
    int time = 1;
    int actionError = 0;
    while(player1->current_x != destination_x && player1->current_y != destination_y && player1->life_left > 0
    && time < SESSION_TIMEOUT) {
        std::cout<<"Time "<<time<<endl;
        std::cout<<"player ("<<player1->current_x<<","<<player1->current_y<<")"<<endl;
        // Next Action
        int action = movePlayer(&actionError);
        grid[player1->current_x][player1->current_y] = 19;
        moveEnemies(enemies);
        ob = player1->createObservation(grid, enemies);
        int reward = calculateReward(enemies, ob, actionError);
        player1->evaluateActionQValues(reward, ob, action, grid);
        player1->cur_state->x = player1->current_x;
        player1->cur_state->y = player1->current_y;
        fight(enemies);
        time++;
        total_rewards += reward;
    }
    std::cout<<"Player 1 life left "<<player1->life_left<<"\n";
}

// TODO: Handle move unavailable - don't change state
int gameSimulation::movePlayer(int* error) {
    int nextAction = player1->getNextAction();
    std::cout<<"NEXT ACTION ";
    printAction(nextAction);
    switch(nextAction) {
        case ACTION_FOLLOW:
            if (!player1->isOnTrack()) {
                *error = -1;
                return nextAction;
            }
            player1->follow();
            break;
        case ACTION_STRAIGHT:
            *error = setStraightActionCoordinates(player1->current_x, player1->current_y, player1->cur_state->direction);
            break;
        case ACTION_DODGE_LEFT:
            *error = setDodgeLeftActionCoordinates(player1->current_x, player1->current_y, player1->cur_state->direction);
            break;
        case ACTION_DODGE_DIAGONAL_LEFT:
            *error = setDodgeDiagonalLeftActionCoordinates(player1->current_x, player1->current_y, player1->cur_state->direction);
            break;
        case ACTION_DODGE_RIGHT:
            *error = setDodgeRightActionCoordinates(player1->current_x, player1->current_y, player1->cur_state->direction);
            break;
        case ACTION_DODGE_DIAGONAL_RIGHT:
            *error = setDodgeDiagonalRightActionCoordinates(player1->current_x, player1->current_y, player1->cur_state->direction);
            break;
    }
    return nextAction;
}

void gameSimulation::moveEnemies(std::vector<enemy>& enemies) {
    for(enemy e: enemies){
        e.doNextMove();
    }
}

void gameSimulation::fight(std::vector<enemy> &enemies) {
    for(enemy e: enemies) {
        //cout<<"fight player at "<<player1.current_x<<", "<<player1.current_y<<"\n";
        //cout<<"enemy at "<<e.current_x<<", "<<e.current_y<<"\n";
        if (e.current_x == player1->current_x && e.current_y == player1->current_y) {
            //cout<<"Player1 damaged"<<"\n";
            player1->takeDamage(e.getAttackPoints());
        }
    }
}


int gameSimulation::calculateReward(vector<enemy> &enemies, observation &ob, int action_error) {
    if(player1->current_x == destination_x && player1->current_y == destination_y) {
        return REWARD_REACH;
    }
    enemy e = enemies[0];
    if(player1->current_x == e.current_x && player1->current_y == e.current_y) {
        return REWARD_DEATH;
    }
    if(action_error == -1) {
        return REWARD_ACTION_UNAVAILABLE;
    }
    if(ob.trajectory == on_track) {
        return REWARD_TRACK_FOLLOW;
    } else if (ob.trajectory >= 11 && ob.trajectory <= 14) {
        return REWARD_TRACK_ONE_DIV;
    } else if (ob.trajectory >= 21 && ob.trajectory <= 24) {
        return REWARD_TRACK_TWO_DIV;
    } else {
        return REWARD_OFFTRACK;
    }
}


int gameSimulation::getTotalRewardsCollected() {
    return total_rewards;
}

void gameSimulation::printAction(int action) {
    switch (action) {
        case ACTION_FOLLOW:
            cout<<"ACTION_FOLLOW"<<endl;
            break;
        case ACTION_DODGE_LEFT:
            cout<<"ACTION_DODGE_LEFT"<<endl;
            break;
        case ACTION_DODGE_RIGHT:
            cout<<"ACTION_DODGE_RIGHT"<<endl;
            break;
        case ACTION_DODGE_DIAGONAL_LEFT:
            cout<<"ACTION_DODGE_DIAGONAL_LEFT"<<endl;
            break;
        case ACTION_DODGE_DIAGONAL_RIGHT:
            cout<<"ACTION_DODGE_DIAGONAL_RIGHT"<<endl;
            break;

    }
}

void gameSimulation::reset(std::vector<std::vector<int>> &grid) {
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            if(grid[i][j] == 19 || grid[i][j] == 1) {
                grid[i][j] = 0;
            }
        }
    }
    total_rewards = 0;
}

void gameSimulation::populateEnemies(vector<std::vector<int>> &grid, vector<enemy> &enemies) {
    for(enemy e: enemies) {
        grid[e.start_x][e.start_y] = e.id;
    }
}
