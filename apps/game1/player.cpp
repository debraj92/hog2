//
// Created by Debraj Ray on 2021-12-30.
//

#include "player.h"
#include "gameSimulation.h"
#include "plot/pbPlots.hpp"
#include "plot/supportLib.hpp"
#include "enemyLocator.h"
#include "gameSimulation.h"

void player::takeDamage(int points) {
    life_left -= points;
}


void player::locateTrajectoryAndDirection(observation &ob) {
    int row, col;
    for (int i=1; i<=VISION_RADIUS; i++) {
        row = current_x - i;
        if (row > 0) {
            for(col=current_y - i; col<=current_y + i; col++) {
                if (col > 0 && col < GRID_SPAN - 1) {
                    pair<int, int> trajectory_vertex = std::make_pair(row, col);
                    if(fp.all_vertices.find(trajectory_vertex) != fp.all_vertices.end()) {
                        ob.direction = fp.all_vertices.find(trajectory_vertex)->second;
                        ob.trajectory = (i*10) + 1;
                    }
                }
            }
        }
        row = current_x + i;
        if (row < GRID_SPAN - 1) {
            for(col=current_y - i; col<=current_y + i; col++) {
                if (col > 0 && col < GRID_SPAN - 1) {
                    pair<int, int> trajectory_vertex = std::make_pair(row, col);
                    if(fp.all_vertices.find(trajectory_vertex) != fp.all_vertices.end()) {
                        ob.direction = fp.all_vertices.find(trajectory_vertex)->second;
                        ob.trajectory = (i*10) + 2;
                    }
                }
            }
        }
        col = current_y - i;
        if (col > 0) {
            for(row=current_x - i; row<=current_x + i; row++) {
                if (row > 0 && row < GRID_SPAN - 1) {
                    pair<int, int> trajectory_vertex = std::make_pair(row, col);
                    if(fp.all_vertices.find(trajectory_vertex) != fp.all_vertices.end()) {
                        ob.direction = fp.all_vertices.find(trajectory_vertex)->second;
                        ob.trajectory = (i*10) + 3;
                    }
                }
            }
        }
        col = current_y + i;
        if (col < GRID_SPAN - 1) {
            for(row=current_x - i; row<=current_x + i; row++) {
                if (row > 0 && row < GRID_SPAN - 1) {
                    pair<int, int> trajectory_vertex = std::make_pair(row, col);
                    if(fp.all_vertices.find(trajectory_vertex) != fp.all_vertices.end()) {
                        ob.direction = fp.all_vertices.find(trajectory_vertex)->second;
                        ob.trajectory = (i*10) + 4;
                    }
                }
            }
        }
    }
}

void player::learnGame(vector<std::vector<int>> &grid, vector<enemy> &enemies) {
    vector<double> rewards;
    vector<double> episodes;
    gameSimulation game;
    game.player1 = this;
    for(int i=1; i<MAX_EPISODES; i++) {
        cout<<"EPOCH "<<i<<endl;
        game.learnToPlay(grid, enemies);
        printAllStatesAndPolicies();
        cout<<endl;
        printBoard(grid);
        cout<<"Total rewards collected "<<game.getTotalRewardsCollected()<<endl;
        rewards.push_back(game.getTotalRewardsCollected());
        episodes.push_back(i);
        reset(grid);
        game.reset(grid);
        make_greedy();
    }

    RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();
    StringReference *errorMessage = new StringReference();
    auto success = DrawScatterPlot(imageReference, 1000, 1000, &episodes, &rewards, errorMessage);
    if(success){
        vector<double> *pngdata = ConvertToPNG(imageReference->image);
        WriteToFile(pngdata, "/Users/debrajray/MyComputer/RTS/plot/output.png");
        DeleteImage(imageReference->image);
    }else{
        cerr << "Error: ";
        for(wchar_t c : *errorMessage->string){
            wcerr << c;
        }
        cerr << endl;
    }

}

observation player::createObservation(std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies) {
    cout<<"player::createObservation"<<endl;
    observation ob;
    ob.direction = getDirection();
    if (!isOnTrack()) {
        cout<<"("<<current_x<<", "<<current_y<<") is not on track"<<endl;
        locateTrajectoryAndDirection(ob);
    } else {
        cout<<"("<<current_x<<", "<<current_y<<") is on track"<<endl;
        ob.trajectory = on_track;
    }
    enemyLocator el;
    for(const enemy& e: enemies) {
        el.locateEnemy(current_x, current_y, ob.direction, e.current_x, e.current_y);
        double distance = el.getEnemyDistance();
        if (distance <= VISION_RADIUS * sqrt(2)) {
            ob.enemy_distance = distance;
            ob.enemy_cosine = el.getEnemyCosine();
        }
    }
    ob.updateObstacleDistances(grid, current_x, current_y);
    return ob;
}

void player::reset(std::vector<std::vector<int>> &grid) {
    current_x = 0;
    current_y = 0;
    life_left = 10;
    fp.reset();
}

int player::getDirection() {
    return fp.all_vertices.find(make_pair(current_x, current_y))->second;
}

void player::findPathToDestination() {
    fp.findPathToDestination();
}

void player::follow() {
    fp.calculateNextPosition(current_x, current_y);
    current_x = fp.getNext_x();
    current_y = fp.getNext_y();
}

bool player::isOnTrack() {
    return fp.all_vertices.find(make_pair(current_x, current_y)) != fp.all_vertices.end();
}

void player::evaluateActionQValues(int reward, observation &next_observation, int current_action, std::vector<std::vector<int>> &grid) {
    cur_state = evaluateActionProbabilities(reward, next_observation, current_action);
    ontrack = next_observation.trajectory == on_track;
    //cur_state->updateObstacleDistances(grid, current_x, current_y);
}

void player::printBoard(std::vector<std::vector<int>> &grid) {
    cout<<"print board"<<endl;
    for (int row=0; row<GRID_SPAN; row++) {
        for (int col=0; col<GRID_SPAN; col++) {
            cout<<grid[row][col]<<" ";
        }
        cout<<"\n";
    }
}
