//
// Created by Debraj Ray on 2021-12-30.
//

#include "player.h"

#include <memory>
#include "gameSimulation.h"
#include "plot/pbPlots.hpp"
#include "plot/supportLib.hpp"

#include <random>
#include <iostream>

void player::takeDamage(int points) {
    life_left -= points;
}

void player::learnGame(vector<std::vector<int>> &grid, vector<enemy> &enemies) {
    vector<double> rewards;
    vector<double> episodes;
    gameSimulation game;
    game.player1 = this;


    int sources[11][2] = {
            {0, 0},
            {GRID_SPAN-1, GRID_SPAN-1},
            {0, (GRID_SPAN)/2},
            {1, (GRID_SPAN)/4},
            {GRID_SPAN-1, (GRID_SPAN)/2},
            {(GRID_SPAN)/2, GRID_SPAN-1},
            {0, GRID_SPAN-1},
            {(GRID_SPAN)/2,0},
            {(GRID_SPAN)/3,GRID_SPAN-1},
            {GRID_SPAN-2, GRID_SPAN-1},
            {GRID_SPAN-1, GRID_SPAN-1}
    };
    int destinations[11][2] = {
            {GRID_SPAN-1, GRID_SPAN-1},
            {0, 0},
            {GRID_SPAN-1, (GRID_SPAN) / 2},
            {GRID_SPAN-1, GRID_SPAN-1},
            {0, (GRID_SPAN)/2},
            {(GRID_SPAN)/2, 0},
            {GRID_SPAN-1, 2},
            {(GRID_SPAN)/3,GRID_SPAN-1},
            {0, (GRID_SPAN)/3},
            {1, 0},
            {1, 0}
    };


    /*
     * TODO: Will work only when dodge is perfectly implemented - no getting lost
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> randomGen(0,GRID_SPAN-1);
     */


    for(int i=1; i<MAX_EPISODES; i++) {
        // pick a random source and destination
        int src_x, src_y, dest_x, dest_y;

        //game.player1->initialize(sources[i%11][0], sources[i%11][1], destinations[i%11][0], destinations[i%11][1]);
        game.player1->initialize(sources[0][0], sources[0][1], destinations[0][0], destinations[0][1]);
        //selectRandomSourceAndDestinationCoordinates(rng, randomGen, grid, src_x, src_y, dest_x, dest_y);
        //game.player1->initialize(src_x, src_y, dest_x, dest_y);

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

void player::playGame(vector<std::vector<int>> &grid, vector<enemy> &enemies, int src_x, int src_y, int dest_x, int dest_y) {
    gameSimulation game;
    game.player1 = this;
    cout<<"Source ("<<src_x<<", "<<src_y<<") Destination ("<<dest_x<<", "<<dest_y<<")"<<endl;
    this->initialize(src_x, src_y, dest_x, dest_y);
    game.play(grid, enemies);
    cout<<endl;
    printBoard(grid);
    reset(grid);
    game.reset(grid);
}

observation player::createObservation(std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies) {
    cout<<"player::createObservation"<<endl;
    observation ob;
    if (!isOnTrack()) {
        cout<<"("<<current_x<<", "<<current_y<<") is not on track"<<endl;
        ob.locateTrajectoryAndDirection(fp, current_x, current_y);
    } else {
        cout<<"("<<current_x<<", "<<current_y<<") is on track"<<endl;
        ob.trajectory = on_track;
        ob.direction = getDirection();
    }

    if (ob.direction != 0) {
        ob.locateEnemies(enemies, current_x, current_y);
        ob.updateObstacleDistances(grid, current_x, current_y);
    }
    return ob;
}

void player::reset(std::vector<std::vector<int>> &grid) {
    current_x = source_x;
    current_y = source_y;
    life_left = MAX_LIFE;
}

int player::getDirection() {
    return fp->pathDirection(current_x, current_y);
}

void player::findPathToDestination(std::vector<std::vector<int>> &grid, int src_x, int src_y, int dst_x, int dst_y) {
    cout<<"Find path to destination"<<endl;
    fp = std::make_shared<findPath>(grid, src_x, src_y, dst_x, dst_y);
    fp->findPathToDestination();
}

void player::follow() {
    fp->calculateNextPosition(current_x, current_y);
    current_x = fp->getNext_x();
    current_y = fp->getNext_y();
}

bool player::isOnTrack() {
    return fp->isOnTrack(current_x, current_y);
}

void player::evaluateActionQValues(int reward, observation &next_observation, int current_action) {
    cur_state = evaluateActionProbabilities(reward, next_observation, current_action);
    ontrack = next_observation.trajectory == on_track;
    //cur_state->updateObstacleDistances(grid, current_x, current_y);
}

void player::getNextStateForInference(observation &next_observation) {
    cur_state = getNextStateFromObservation(next_observation);
    ontrack = next_observation.trajectory == on_track;
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

void player::initialize(int src_x, int src_y, int dest_x, int dest_y) {
    source_x = src_x;
    source_y = src_y;
    destination_x = dest_x;
    destination_y = dest_y;
    life_left = MAX_LIFE;
    ontrack = true;
    current_x = source_x;
    current_y = source_y;

}

void player::selectRandomSourceAndDestinationCoordinates(std::mt19937 &rng, std::uniform_int_distribution<std::mt19937::result_type> &randGen, std::vector<std::vector<int>> &grid, int &src_x, int &src_y, int &dest_x, int &dest_y) {
    bool src_generated = false;
    while(!src_generated) {
        src_x = randGen(rng);
        src_y = randGen(rng);
        if(grid[src_x][src_y]==0) {
            src_generated = true;
        }
    }

    bool dest_generated = false;
    while(!dest_generated) {
        dest_x = randGen(rng);
        dest_y = randGen(rng);
        if(grid[dest_x][dest_y]==0 && !(dest_x == src_x && dest_y == src_y)) {
            dest_generated = true;
        }
    }

    cout<<"Source ("<<src_x<<", "<<src_y<<") Destination ("<<dest_x<<", "<<dest_y<<")"<<endl;
}


