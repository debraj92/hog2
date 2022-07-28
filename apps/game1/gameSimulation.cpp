//
// Created by Debraj Ray on 2021-12-30.
//

#include "gameSimulation.h"


using namespace std;

void gameSimulation::play(vector<std::vector<int>> &grid, vector<enemy> &enemies) {
    logger->logDebug("gameSimulation::play")->endLineDebug();
    populateEnemies(grid, enemies);
    if (not player1->isSimpleAstarPlayer) {
        bool isPathFound = player1->findPathToDestination(grid, enemies, player1->current_x, player1->current_y, player1->destination_x, player1->destination_y);
        if (not isPathFound) {
            logger->logInfo("No path found, ignoring navigation")->endLineInfo();
            return;
        }
    }
    grid[player1->current_x][player1->current_y] = 9;
    logger->printBoardDebug(grid);
    int time = 1;
    int actionError = 0;
    observation currentObservation;
    player1->observe(currentObservation, grid, enemies, ACTION_STRAIGHT, false, 0);
    int action = ACTION_STRAIGHT;
    while((not isEpisodeComplete()) && time <= SESSION_TIMEOUT) {
        logger->logDebug("Time ")->logDebug(time)->endLineDebug();
        logger->logDebug("player (" + to_string(player1->current_x) + ", "+to_string(player1->current_y)+")")->endLineDebug();
        // Next Action
        action = movePlayer(grid, enemies, currentObservation, &actionError);
        fight(enemies, grid);
        // Enemy operations
        if (player1->life_left > 0 and not isDestinationReached()) {
            moveEnemies(enemies, grid, currentObservation, time);
            fight(enemies, grid);
        }
        logger->printBoardDebug(grid);
        // Observe next State
        observation nextObservation;
        player1->observe(nextObservation, grid, enemies, action, currentObservation.isPlayerInHotPursuit, currentObservation.direction);
        if (nextObservation.trajectory_off_track) {
            // poisoned if off track
            player1->life_left = 0;
        }
        // Next state reward
        auto reward = calculateReward(nextObservation, action, actionError);
        logger->logDebug("Reward received ")->logDebug(reward)->endLineDebug();
        currentObservation = nextObservation;
        time++;
        player1->total_rewards += reward;
        if (currentObservation.isGoalInSight and player1->life_left > 0) {
            logger->logDebug("Marching towards destination")->endLineDebug();
            headStraightToDestination(grid, enemies);
        }
    }
    logger->logDebug("Player 1 life left ")->logDebug(player1->life_left)->endLineDebug();
}

void gameSimulation::learnToPlay(std::vector<std::vector<int>> &grid, std::vector<enemy> &enemies) {
    logger->logDebug("gameSimulation::learnToPlay")->endLineDebug();
    populateEnemies(grid, enemies);
    bool isPathFound = player1->findPathToDestination(grid, enemies, player1->current_x, player1->current_y, player1->destination_x, player1->destination_y);
    if (not isPathFound) {
        logger->logInfo("No path found, ignoring training");
        return;
    }
    grid[player1->current_x][player1->current_y] = 9;
    logger->printBoardDebug(grid);
    observation currentObservation;
    player1->observe(currentObservation, grid, enemies, ACTION_STRAIGHT, false, 0);
    int time = 1;
    while((not isEpisodeComplete()) && time <= SESSION_TIMEOUT) {
        logger->logDebug("Time ")->logDebug(time)->endLineDebug();
        logger->logDebug("player (" + to_string(player1->current_x) + ", "+to_string(player1->current_y)+")")->endLineDebug();
        int actionError = 0;
        // Next Action
        int action = movePlayer(grid, enemies, currentObservation, &actionError);
        fight(enemies, grid);
        if (player1->life_left > 0 and not isDestinationReached()) {
            moveEnemies(enemies, grid, currentObservation, time);
            fight(enemies, grid);
        }
        logger->printBoardDebug(grid);
        // Observe after action
        observation nextObservation;
        player1->observe(nextObservation, grid, enemies, action, currentObservation.isPlayerInHotPursuit, currentObservation.direction);
        if (nextObservation.trajectory_off_track) {
            // poisoned if off track
            player1->life_left = 0;
        }
        auto reward = calculateReward(nextObservation, action, actionError);
        logger->logDebug("Reward received ")->logDebug(reward)->endLineDebug();
        player1->memorizeExperienceForReplay(currentObservation, nextObservation, action, reward, isMDPDone(nextObservation));
        currentObservation = nextObservation;
        time++;
        player1->total_rewards += reward;
        if (currentObservation.isGoalInSight and player1->life_left > 0) {
            logger->logDebug("Marching towards destination")->endLineDebug();
            headStraightToDestination(grid, enemies);
        }
    }
    logger->logDebug("Player 1 life left ")->logDebug(player1->life_left)->endLineDebug();
    if (player1->life_left <= 0 and not player1->stopLearning) {
        player1->playerDiedInPreviousEpisode = player1->recordRestoreLocation(enemies);
    } else {
        player1->playerDiedInPreviousEpisode = false;
    }
}


int gameSimulation::movePlayer(vector<vector<int>> &grid, std::vector<enemy>& enemies, const observation &currentObservation, int* error) {

    auto savedLocationPlayerX = player1->current_x;
    auto savedLocationPlayerY = player1->current_y;
    int nextAction;
    if (player1->isSimpleAstarPlayer) {
        nextAction = ACTION_STRAIGHT;
    } else {
        nextAction = player1->selectAction(currentObservation);
    }

    switch(nextAction) {
        case ACTION_STRAIGHT:
            *error = setStraightActionCoordinates(player1->current_x, player1->current_y, currentObservation.direction);
            break;
        case ACTION_DODGE_DIAGONAL_LEFT:
            *error = setDodgeDiagonalLeftActionCoordinates(player1->current_x, player1->current_y, currentObservation.direction);
            break;
        case ACTION_DODGE_DIAGONAL_RIGHT:
            *error = setDodgeDiagonalRightActionCoordinates(player1->current_x, player1->current_y, currentObservation.direction);
            break;
        case ACTION_DODGE_LEFT:
            *error = setDodgeLeftActionCoordinates(player1->current_x, player1->current_y, currentObservation.direction);
            break;
        case ACTION_DODGE_RIGHT:
            *error = setDodgeRightActionCoordinates(player1->current_x, player1->current_y, currentObservation.direction);
            break;
        default:
            logger->logInfo("ERROR: Wrong next action")->endLineInfo();
    }

    grid[savedLocationPlayerX][savedLocationPlayerY] = 0;
    grid[player1->current_x][player1->current_y] = 9;

    return nextAction;
}

void gameSimulation::moveEnemies(std::vector<enemy>& enemies, vector<std::vector<int>> &grid, observation &ob, int time) {
    for(enemy &e: enemies){
        // ignore dead enemies
        e.doNextMove(time, grid, {ob.playerX, ob.playerY, 0});
    }
}

void gameSimulation::fight(std::vector<enemy> &enemies, vector<std::vector<int>> &grid) {
    std::unordered_map<node_, int, node_::node_Hash> enemyLocations;
    // damage player
    for(enemy &e: enemies) {
        // ignore dead enemies
        if (e.getLifeLeft() > 0) {
            if (e.current_x == player1->current_x && e.current_y == player1->current_y) {
                logger->logDebug("Player killed at (")->logDebug(player1->current_x)->logDebug(",")->logDebug(player1->current_y)
                        ->logDebug(")")->endLineDebug();
                player1->takeDamage(e.getAttackPoints());
                // enemy is also killed
                e.takeDamage(e.getAttackPoints());
            }
            auto enemyLocation = enemyLocations.find(node_(e.current_x, e.current_y));
            if (enemyLocation != enemyLocations.end()) {
                enemyLocation->second++;
            } else {
                enemyLocations.insert(make_pair(node_(e.current_x, e.current_y), 1));
            }
        }
    }

    // damage enemies
    for(auto enemy_iterator = enemies.begin(); enemy_iterator != enemies.end();) {
        if (enemy_iterator->getLifeLeft() > 0) {
            if (enemyLocations.find(node_(enemy_iterator->current_x, enemy_iterator->current_y))->second >= 2) {
                logger->logDebug("Enemy killed, id: ")->logDebug(enemy_iterator->id)->endLineDebug();
                enemy_iterator->takeDamage(enemy_iterator->getAttackPoints());
                grid[enemy_iterator->current_x][enemy_iterator->current_y] = 0;
            }
        }
        if (enemy_iterator->getLifeLeft() <= 0) {
            // clean up dead enemies
            grid[enemy_iterator->current_x][enemy_iterator->current_y] = 0;
            enemies.erase(enemy_iterator);
        } else {
            ++enemy_iterator;
        }
    }
}

float gameSimulation::calculateReward(const observation &nextObservation, int action, int action_error) {
    if(player1->life_left <= 0) {
        return REWARD_DEATH;
    }
    if(action_error == -1) {
        return REWARD_ACTION_UNAVAILABLE;
    }
    if (action == ACTION_DODGE_LEFT or action == ACTION_DODGE_RIGHT) {
        return REWARD_ACTION_LR;
    }
    if (nextObservation.isPlayerInHotPursuit) {
        return REWARD_TRACK_TWO_DIV;
    }
    if(nextObservation.trajectory == on_track) {
        return REWARD_REACH;
    } else if (nextObservation.trajectory >= lower_bound_one_deviation && nextObservation.trajectory <= upper_bound_one_deviation) {
        return REWARD_TRACK_ONE_DIV;
    } else if (nextObservation.trajectory >= lower_bound_two_deviation && nextObservation.trajectory <= upper_bound_two_deviation) {
        return REWARD_TRACK_TWO_DIV;
    } else {
        // Placeholder - will never hit as player is dead
        return REWARD_OFFTRACK;
    }
}


float gameSimulation::getTotalRewardsCollected() {
    return player1->total_rewards;
}

void gameSimulation::removeCharacters(std::vector<std::vector<int>> &grid) {
    for(int i=0; i<GRID_SPAN; i++) {
        for(int j=0; j<GRID_SPAN; j++) {
            if(grid[i][j] > 0) {
                grid[i][j] = 0;
            }
        }
    }
}

void gameSimulation::populateEnemies(vector<std::vector<int>> &grid, vector<enemy> &enemies) {
    for(enemy e: enemies) {
        if (e.getLifeLeft() > 0) {
            grid[e.start_x][e.start_y] = e.id;
        }
    }
}

bool gameSimulation::isDestinationReached() {
    return  player1->current_x == player1->destination_x and player1->current_y == player1->destination_y;
}

bool gameSimulation::isEpisodeComplete() {
    return isDestinationReached() or player1->life_left <= 0;
}

void gameSimulation::headStraightToDestination(vector<vector<int>> &grid, std::vector<enemy>& enemies) {
    /// No enemies at interface or locations within goal radius
    findPath fp(grid, player1->current_x, player1->current_y, player1->destination_x, player1->destination_y);
    bool pathFound = fp.findPathToDestination();
    if (not pathFound) {
        logger->logInfo("ERROR: Could not find path to destination")->endLineInfo();
        player1->current_x = player1->destination_x;
        player1->current_y = player1->destination_y;
        return;
    }
    fp.visited_x_onpath = player1->current_x;
    fp.visited_y_onpath = player1->current_y;
    while(not isDestinationReached()) {
        fp.calculateNextPosition();
        grid[player1->current_x][player1->current_y] = 0;
        player1->current_x = fp.getNext_x();
        player1->current_y = fp.getNext_y();
        grid[player1->current_x][player1->current_y] = 9;
        logger->printBoardDebug(grid);
        fp.visited_x_onpath = player1->current_x;
        fp.visited_y_onpath = player1->current_y;
    }
}

bool gameSimulation::isMDPDone(observation &nextObservation) {
    bool mdpDone = (not nextObservation.isPlayerInHotPursuit and nextObservation.trajectory_on_track)
            or nextObservation.trajectory_off_track
            or player1->life_left <= 0;
    logger->logDebug("MDPDone ")->logDebug(mdpDone)->endLineDebug();
    return mdpDone;
}
