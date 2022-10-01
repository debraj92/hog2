//
// Created by Debraj Ray on 2022-09-26.
//

#include "SimpleUIView.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "gameConstants.h"
#include <thread>
#include <vector>

using namespace std;

void SimpleUIView::startGameLoop(bool &isGameStopped) {

    sf::RenderWindow window(sf::VideoMode(LENGTH_UI_FRAME, LENGTH_UI_FRAME), "Adversarial Navigation");

    int gameMap[] =
            {
                    0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                    0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    12, 12, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                    12, 12, 12, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    0, 0, 0, 1, 1, 1, 12, 12, 12, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                    0, 0, 0, 1, 1, 1, 12, 12, 12, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 12, 12, 12, 1, 1, 1, 0, 12, 0,
                    0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 12, 12, 12, 1, 1, 1, 0, 12, 0,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    0, 0, 0, 1, 1, 1, 12, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                    0, 0, 0, 1, 1, 1, 12, 12, 12, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 12, 12,
                    0, 0, 0, 1, 1, 1, 12, 12, 12, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 12, 12
            };


    vector<int> tilesEmpty(std::begin(gameMap), std::end(gameMap));
    vector<int> tiles;

    // change the size of the window
    window.setSize(sf::Vector2u(1900, 1600));
    bool firstMove = true;
    bool paused = false;
    while (window.isOpen())
    {
        sf::Event event{};

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                std::unique_lock<mutex> locker(uiBufferSafeReadWrite);
                isClosed = true;
                uiConditions.notify_one();
                isGameStopped = true;
            } else if (event.type == sf::Event::KeyPressed) {
                paused = not paused;
            }

        }

        // critical section
        TileMap map;
        {
            std::unique_lock<mutex> locker(uiBufferSafeReadWrite);
            if(not paused and not isDrawn) {
                // redraw canvas
                tiles.clear();
                std::copy(tilesEmpty.begin(), tilesEmpty.end(), back_inserter(tiles));
                updateMapWithCurrentState(tiles);
                isDrawn = true;
                if (not firstMove) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(650));
                } else {
                    firstMove = false;
                }

                uiConditions.notify_one();
            }
            drawOnCanvas(map, tiles);
        }
        window.clear();
        window.draw(map);
        window.display();
    }
}

void SimpleUIView::populateInfo(int plX, int plY, int destX, int destY, int pl_life, vector<enemyUIData> &enemies) {
    // critical
    std::unique_lock<mutex> locker(uiBufferSafeReadWrite);
    if (isClosed) {
        return;
    }

    destinationX = destX;
    destinationY = destY;
    playerChangedOrientation = (playerY != -1) and (playerY != plY);
    if (playerChangedOrientation) {
        playerOrientationLeft = plY < playerY;
    }
    playerX = plX;
    playerY = plY;
    isPlayerDead = pl_life <= 0;
    playerReached = playerX == destinationX and playerY == destinationY;
    enemiesInThisRound.clear();
    std::copy(enemies.begin(), enemies.end(), back_inserter(enemiesInThisRound));

    isDrawn = false;
    uiConditions.wait(locker);
}


void SimpleUIView::drawOnCanvas(TileMap &map, vector<int> &tiles) {
    if (!map.load(TILES_RESOURCES, sf::Vector2u(32, 32), &tiles[0], 27, 27))
    {
        logger->logInfo("Error loading game tiles")->endLineInfo();
        return;
    }
}

void SimpleUIView::updateMapWithCurrentState(vector<int> &tiles) {
    if (isPlayerDead) {
        tiles[convert2DIndexTo1D(playerX, playerY)] = 8;
        tiles[convert2DIndexTo1D(destinationX, destinationY)] = 6;
    } else if (playerReached) {
        tiles[convert2DIndexTo1D(destinationX, destinationY)] = 7;
    } else {
        tiles[convert2DIndexTo1D(destinationX, destinationY)] = 6;
        if (playerOrientationLeft) {
            tiles[convert2DIndexTo1D(playerX, playerY)] = 5;
        } else {
            tiles[convert2DIndexTo1D(playerX, playerY)] = 4;
        }
    }

    for(enemyUIData enemy: enemiesInThisRound) {
        if(enemy.isDead) {
            if(enemy.x != playerX or enemy.y != playerY) {
                tiles[convert2DIndexTo1D(enemy.x, enemy.y)] = 9;
            }
        } else {
            if(enemy.isOrientationLeft) {
                tiles[convert2DIndexTo1D(enemy.x, enemy.y)] = 10;
            } else {
                tiles[convert2DIndexTo1D(enemy.x, enemy.y)] = 11;
            }
        }
    }

}

int SimpleUIView::convert2DIndexTo1D(int x, int y) {
    return x * GRID_SPAN + y;
}
