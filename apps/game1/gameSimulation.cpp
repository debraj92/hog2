//
// Created by Debraj Ray on 2021-12-30.
//

#include "gameSimulation.h"

using namespace std;

void gameSimulation::play(vector<std::vector<int>> &grid, vector<enemy> &enemies) {
    logger->logDebug("gameSimulation::play")->endLineDebug();
/*
    populateEnemies(grid, enemies);
    player1->findPathToDestination(grid, enemies, player1->current_x, player1->current_y, player1->destination_x, player1->destination_y);
    grid[player1->current_x][player1->current_y] = 9;
    int time = 1;
    int actionError = 0;
    observation ob;
    player1->observe(ob, grid, enemies, false);
    while(!isDestinationReached() && player1->life_left > 0 && time < SESSION_TIMEOUT) {
        std::cout<<"Time "<<time<<endl;
        std::cout<<"player ("<<player1->current_x<<","<<player1->current_y<<")"<<endl;
        // Observe the state and take an action
        player1->getNextStateForInference(ob);
        player1->cur_state->x = player1->current_x;
        player1->cur_state->y = player1->current_y;
        // Next Action
        int action = movePlayer(grid, enemies, ob, &actionError, true);
        grid[player1->current_x][player1->current_y] = 9;
        player1->printBoard(grid);
        moveEnemies(enemies);
        ob = createObservationAfterAction(grid, enemies, ob, action);
        fight(enemies);
        int reward = calculateReward(enemies, ob, action, actionError);
        player1->total_rewards += reward;
        time++;
    }
    std::cout<<"Player 1 life left "<<player1->life_left<<"\n";
*/
}


void gameSimulation::learnToPlay(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies) {
    logger->logDebug("gameSimulation::learnToPlay")->endLineDebug();
    populateEnemies(grid, enemies);
    player1->findPathToDestination(grid, enemies, player1->current_x, player1->current_y, player1->destination_x, player1->destination_y);
    grid[player1->current_x][player1->current_y] = 9;
    logger->printBoard(grid);
    observation currentObservation;
    player1->observe(currentObservation, grid, enemies, false);
    int time = 1;
    double loss_sum = 0;
    int loss_count = 0;
    while((not isEpisodeComplete()) && time <= SESSION_TIMEOUT) {
        logger->logDebug("Time ")->logDebug(time)->endLineDebug();
        logger->logDebug("player (" + to_string(player1->current_x) + ", "+to_string(player1->current_y)+")")->endLineDebug();
        int actionError = 0;
        // Next Action
        int action = movePlayer(grid, enemies, currentObservation, &actionError);
        logger->printBoard(grid);
        moveEnemies(enemies);
        observation nextObservation = createObservationAfterAction(grid, enemies, currentObservation, action);
        fight(enemies);
        auto reward = calculateReward(enemies, nextObservation, action, actionError);
        logger->logDebug("Reward received ")->logDebug(reward)->endLineDebug();
        player1->memorizeExperienceForReplay(currentObservation, nextObservation, action, reward, isEpisodeComplete());
        loss_sum += player1->learnWithDQN();
        loss_count ++;
        currentObservation = nextObservation;
        time++;
        player1->total_rewards += reward;
    }
    double avg_loss = loss_sum / loss_count;
    logger->logDebug("Player 1 life left ")->logDebug(player1->life_left)->endLineDebug();
    if (not player1->stopLearning) {
        logger->logInfo("Network Loss after episode completion ")->logInfo(avg_loss)->endLineInfo();
    }
}


int gameSimulation::movePlayer(vector<vector<int>> &grid, std::vector<enemy>& enemies, observation &currentObservation, int* error) {

    int oldLocationX = player1->current_x;
    int oldLocationY = player1->current_y;

    //std::cout<<"NEXT ACTION ";
    logger->logDebug("NEXT ACTION: ")->endLineDebug();
    int nextAction = player1->selectAction(currentObservation);
    if (nextAction != ACTION_SWITCH) {
        currentObservation.resetRerouteDistance();
    }
    switch(nextAction) {
        case ACTION_STRAIGHT:
            *error = setStraightActionCoordinates(player1->current_x, player1->current_y, currentObservation.direction);
            break;
        case ACTION_DODGE_LEFT:
            *error = setDodgeLeftActionCoordinates(player1->current_x, player1->current_y, currentObservation.direction);
            break;
        case ACTION_DODGE_DIAGONAL_LEFT:
            *error = setDodgeDiagonalLeftActionCoordinates(player1->current_x, player1->current_y, currentObservation.direction);
            break;
        case ACTION_DODGE_RIGHT:
            *error = setDodgeRightActionCoordinates(player1->current_x, player1->current_y, currentObservation.direction);
            break;
        case ACTION_DODGE_DIAGONAL_RIGHT:
            *error = setDodgeDiagonalRightActionCoordinates(player1->current_x, player1->current_y, currentObservation.direction);
            break;
        case ACTION_REROUTE:
            player1->findNewRoute(grid, currentObservation, enemies, player1->current_x, player1->current_y, player1->destination_x, player1->destination_y);
            break;
        case ACTION_REDIRECT:
            currentObservation.redirect(player1->current_x, player1->current_y, player1->destination_x, player1->destination_y);
            break;
        case ACTION_SWITCH:
            *error = player1->switchToNewRoute(currentObservation);
            currentObservation.resetRerouteDistance();
            break;
    }
    grid[oldLocationX][oldLocationY] = 0;
    grid[player1->current_x][player1->current_y] = 9;
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
            logger->logDebug("Player1 damaged")->endLineDebug();
            player1->takeDamage(e.getAttackPoints());
        }
    }
}


float gameSimulation::calculateReward(vector<enemy> &enemies, observation &ob, int action, int action_error) {
    if(isDestinationReached()) {
        return REWARD_REACH;
    }
    if(ob.playerLifeLeft <= 0) {
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


float gameSimulation::getTotalRewardsCollected() {
    return player1->total_rewards;
}

void gameSimulation::reset(std::vector<std::vector<int>> &grid) {
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            if(grid[i][j] > 0) {
                grid[i][j] = 0;
            }
        }
    }
    player1->total_rewards = 0;
}

void gameSimulation::populateEnemies(vector<std::vector<int>> &grid, vector<enemy> &enemies) {
    for(enemy e: enemies) {
        grid[e.start_x][e.start_y] = e.id;
    }
}

bool gameSimulation::isDestinationReached() {
    return  player1->current_x == player1->destination_x and player1->current_y == player1->destination_y;
}


observation gameSimulation::createObservationAfterAction(vector<vector<int>> &grid, std::vector<enemy>& enemies, observation ob, int action) {
    switch(action) {
        case ACTION_REDIRECT:
        {
            observation ob1;
            ob1.direction = ob.direction;
            ob1.trajectory = ob.trajectory;
            player1->observe(ob1, grid, enemies, true);
            return ob1;
        }
        case ACTION_REROUTE:
        {
            observation ob1;
            ob1.rerouteDistance = ob.rerouteDistance;
            player1->observe(ob1, grid, enemies, false);
            return ob1;
        }
        default:
        {
            observation ob1;
            player1->observe(ob1, grid, enemies, false);
            return ob1;
        }

    }

}

bool gameSimulation::isEpisodeComplete() {
    return isDestinationReached() or player1->life_left <= 0;
}
