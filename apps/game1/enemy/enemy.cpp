//
// Created by Debraj Ray on 2021-12-30.
//
#include "enemy.h"
#include "../coordinatesUtil.h"
#include "chrono"

int enemy::getAttackPoints() {
    return 10;
}

int enemy::getLifeLeft() {
    return life_left;
}

void enemy::takeDamage(int points) {
    life_left -= points;
}

bool enemy::isPlayerInSight() {
    return fp->getShortestDistanceToDestination() <= static_cast<float>(enemyVisionRadius);
}

void enemy::doNextMove(vector<std::vector<int>> &grid, enemy::playerInfo pl_info) {
    if(isFixed) {
        return;
    }
    fp = std::make_shared<findPath>(grid, current_x, current_y, pl_info.player_x, pl_info.player_y);

    if(isPlayerInSight()) {
        int playerDistanceFromLastKnownLocation = max(abs(pl_info.player_x - lastKnownPlayerX), abs(pl_info.player_y - lastKnownPlayerY));
        if (lastKnownPlayerX > 0 and playerDistanceFromLastKnownLocation == 1) {
            pl_info.player_direction = fp->inferDirection(lastKnownPlayerX, lastKnownPlayerY, pl_info.player_x, pl_info.player_y);
            lastKnownPlayerX = pl_info.player_x;
            lastKnownPlayerY = pl_info.player_y;
            predictNextPlayerLocation(grid, pl_info);
            fp->changeSourceAndDestination(current_x, current_y, pl_info.player_x, pl_info.player_y);
        } else {
            lastKnownPlayerX = pl_info.player_x;
            lastKnownPlayerY = pl_info.player_y;
        }


        logger->logDebug("Player in sight of enemy: ")->logDebug(id)->endLineDebug();
        bool pathFound;
        if (pl_info.player_direction > 0) {
            pathFound = fp->findPathToDestination(pl_info.player_direction);
        } else {
            pathFound = fp->findPathToDestination();
        }
        if (not pathFound) {
            logger->logInfo("ERROR: path to player not found, enemy-id:")->logDebug(id)->endLineInfo();
            return;
        }
        grid[current_x][current_y] = 0;
        fp->getNextPositionAfterGivenLocation(current_x, current_y, current_x, current_y);
        grid[current_x][current_y] = id;
        max_moves--;
        isFixed = max_moves == 0;
    }
}

void enemy::predictNextPlayerLocation(vector<std::vector<int>> &grid, playerInfo &pl_info) {
    logger->logDebug("predictNextPlayerLocation")->endLineDebug();
    if (pl_info.player_direction <= 0) {
        // direction was not inferred correctly
        // target current player location
        return;
    }
    coordinatesUtil cu(grid);
    int playerX = pl_info.player_x;
    int playerY = pl_info.player_y;
    int error = cu.setStraightActionCoordinates(playerX, playerY, pl_info.player_direction);
    if (error == -1) {
        // straight unavailable
        // target current player location
        return;
    }
    if (playerX == current_x and playerY == current_y) {
        // player next to enemy
        playerX = pl_info.player_x;
        playerY = pl_info.player_y;
        // randomly choose left-diagonal or right-diagonal of player
        const auto p1 = std::chrono::system_clock::now();
        long long isLeft = (std::chrono::duration_cast<std::chrono::seconds>(
                p1.time_since_epoch()).count()%10) % 2 == 0;
        if (isLeft) {
            error = cu.setDodgeDiagonalLeftActionCoordinates(playerX, playerY, pl_info.player_direction);
        } else {
            error = cu.setDodgeDiagonalRightActionCoordinates(playerX, playerY, pl_info.player_direction);
        }
        if (error != -1) {
            pl_info.player_x = playerX;
            pl_info.player_y = playerY;
        } else {
            // choose the opposite
            playerX = pl_info.player_x;
            playerY = pl_info.player_y;
            if (not isLeft) {
                error = cu.setDodgeDiagonalLeftActionCoordinates(playerX, playerY, pl_info.player_direction);
            } else {
                error = cu.setDodgeDiagonalRightActionCoordinates(playerX, playerY, pl_info.player_direction);
            }
            if (error != -1) {
                pl_info.player_x = playerX;
                pl_info.player_y = playerY;
            }
        }
        return;
    }
    pl_info.player_x = playerX;
    pl_info.player_y = playerY;
}
