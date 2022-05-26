//
// Created by Debraj Ray on 2021-12-30.
//

#include "gameSimulation.h"


void gameSimulation::play(vector<std::vector<int>> &grid, vector<enemy> &enemies) {
    cout<<"gameSimulation::play"<<endl;
    // TODO: Handle redirect
    /*
    populateEnemies(grid, enemies);
    player1->findPathToDestination(grid, enemies, player1->current_x, player1->current_y, player1->destination_x, player1->destination_y);
    grid[player1->current_x][player1->current_y] = 9;
    int time = 1;
    int actionError = 0;
    while(!isDestinationReached() && player1->life_left > 0 && time < SESSION_TIMEOUT) {
        std::cout<<"Time "<<time<<endl;
        std::cout<<"player ("<<player1->current_x<<","<<player1->current_y<<")"<<endl;
        // Observe the state and take an action
        observation ob;
        player1->observe(ob, grid, enemies);
        player1->getNextStateForInference(ob);
        player1->cur_state->x = player1->current_x;
        player1->cur_state->y = player1->current_y;
        // Next Action
        movePlayer(grid, enemies, ob, &actionError, true);
        grid[player1->current_x][player1->current_y] = 9;
        player1->printBoard(grid);
        moveEnemies(enemies);
        fight(enemies);
        time++;
    }
    std::cout<<"Player 1 life left "<<player1->life_left<<"\n";
     */
}


void gameSimulation::learnToPlay(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies) {
    cout<<"gameSimulation::learnToPlay"<<endl;
    populateEnemies(grid, enemies);
    player1->findPathToDestination(grid, enemies, player1->current_x, player1->current_y, player1->destination_x, player1->destination_y);
    grid[player1->current_x][player1->current_y] = 9;
    player1->printBoard(grid);
    player1->ontrack = true;
    observation ob;
    player1->observe(ob, grid, enemies, false);
    player1->createStartState(ob);
    player1->cur_state->x = player1->current_x;
    player1->cur_state->y = player1->current_y;
    //player1->cur_state->updateObstacleDistances(grid, player1->current_x, player1->current_y);
    int time = 1;
    int actionError = 0;
    while(!isDestinationReached() && player1->life_left > 0 && time < SESSION_TIMEOUT) {
        std::cout<<"Time "<<time<<endl;
        std::cout<<"player ("<<player1->current_x<<","<<player1->current_y<<")"<<endl;
        // Next Action
        int action = movePlayer(grid, enemies, ob, &actionError);
        grid[player1->current_x][player1->current_y] = 9;
        player1->printBoard(grid);
        moveEnemies(enemies);
        if (action != ACTION_REDIRECT) {
            ob = observation();
            player1->observe(ob, grid, enemies, false);
        } else {
            // Direction is frozen from the last action
            observation ob1;
            ob1.direction = ob.direction;
            ob1.trajectory = ob.trajectory;
            player1->observe(ob1, grid, enemies, true);
        }
        fight(enemies);
        int reward = calculateReward(enemies, ob, action, actionError);
        cout<<"Reward received "<<reward<<endl;
        player1->evaluateActionQValues(reward, ob, action);
        player1->cur_state->x = player1->current_x;
        player1->cur_state->y = player1->current_y;
        time++;
        total_rewards += reward;
    }
    std::cout<<"Player 1 life left "<<player1->life_left<<"\n";
}

// TODO: Handle move unavailable - don't change state
int gameSimulation::movePlayer(vector<vector<int>> &grid, std::vector<enemy>& enemies, observation &ob, int* error, bool isInference) {
    int nextAction;
    if (isInference) {
        nextAction = player1->getNextActionForInference();
    } else {
        nextAction = player1->getNextAction();
    }
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
        case ACTION_REROUTE:
            player1->findPathToDestination(grid, enemies, player1->current_x, player1->current_y, player1->destination_x, player1->destination_y);
            // TODO: Re-route should set a flag as optional_distance
            // Action switch should allow following this route
            // The agent should learn when to switch
            // If the next action is not switch, then the flag should be cleared
            // Invalid switch should be penalty, and result in no state change
            // Next action can be either Follow / Switch ... therefore remove the next line
            player1->follow();
            break;
        case ACTION_REDIRECT:
            ob.redirect(player1->current_x, player1->current_y, player1->destination_x, player1->destination_y);
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
            cout<<"Player1 damaged"<<endl;
            player1->takeDamage(e.getAttackPoints());
        }
    }
}


int gameSimulation::calculateReward(vector<enemy> &enemies, observation &ob, int action, int action_error) {
    if(isDestinationReached()) {
        return REWARD_REACH;
    }
    if(player1->life_left <= 0) {
        return REWARD_DEATH;
    }
    if(action_error == -1) {
        return REWARD_ACTION_UNAVAILABLE;
    }
    if (action == ACTION_REROUTE) {
        return REWARD_REROUTE;
    }
    if (action == ACTION_REDIRECT) {
        return REWARD_REDIRECT;
    }
    if(ob.trajectory == on_track) {
        return REWARD_TRACK_FOLLOW;
    } else if (ob.trajectory >= lower_bound_one_deviation && ob.trajectory <= upper_bound_one_deviation) {
        return REWARD_TRACK_ONE_DIV;
    } else if (ob.trajectory >= lower_bound_two_deviation && ob.trajectory <= upper_bound_two_deviation) {
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
        case ACTION_STRAIGHT:
            cout<<"ACTION_STRAIGHT"<<endl;
            break;
        case ACTION_REROUTE:
            cout<<"ACTION_REROUTE"<<endl;
            break;
        case ACTION_REDIRECT:
            cout<<"ACTION_REDIRECT"<<endl;
            break;
        default:
            cout<<"INVALID ACTION "<<action<<endl;
    }
}

void gameSimulation::reset(std::vector<std::vector<int>> &grid) {
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            if(grid[i][j] > 0) {
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

bool gameSimulation::isDestinationReached() {
    return player1->current_x == player1->destination_x && player1->current_y == player1->destination_y;
}
