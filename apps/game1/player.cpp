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
    gameSimulation game(grid);
    game.player1 = this;


    int sources[14][2] = {
            {0, 0},
            {GRID_SPAN-1, GRID_SPAN-1},
            {0, (GRID_SPAN-1)/2},
            {GRID_SPAN-1, 0},
            {(GRID_SPAN-1)/2, GRID_SPAN-1},
            {0, GRID_SPAN-1},
            {(GRID_SPAN-1)/2, 0},
            {(GRID_SPAN-1)/2, 0},
            {0, (GRID_SPAN-1)/2},
            {(GRID_SPAN-1)/2, 0},
            {0, 0},
            {GRID_SPAN-1, 0},
            {GRID_SPAN-1, GRID_SPAN-1},
            {0, GRID_SPAN-1}
    };
    int destinations[14][2] = {
            {GRID_SPAN-1, GRID_SPAN-1},
            {0, (GRID_SPAN-1)/2},
            {GRID_SPAN-1, 0},
            {(GRID_SPAN-1)/2, GRID_SPAN-1},
            {GRID_SPAN-1, (GRID_SPAN-1)/2},
            {GRID_SPAN-1, 0},
            {0, GRID_SPAN-1},
            {(GRID_SPAN-1)/2, GRID_SPAN-1},
            {GRID_SPAN-1, (GRID_SPAN-1)/2},
            {0, (GRID_SPAN-1)/2},
            {GRID_SPAN/2, GRID_SPAN/2},
            {GRID_SPAN/2, GRID_SPAN/2},
            {GRID_SPAN/2, GRID_SPAN/2},
            {GRID_SPAN/2, GRID_SPAN/2}
    };


    /*
     * TODO: Will work only when dodge is perfectly implemented - no getting lost
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> randomGen(0,GRID_SPAN-1);
     */

    train_step = 0;
    for(episodeCount = 1; episodeCount < MAX_EPISODES; episodeCount++) {
        // pick a random source and destination
        int src_x, src_y, dest_x, dest_y;

        //game.player1->initialize(sources[i%14][0], sources[i%14][1], destinations[i%14][0], destinations[i%14][1]);
        game.player1->initialize(sources[0][0], sources[0][1], destinations[0][0], destinations[0][1]);
        //selectRandomSourceAndDestinationCoordinates(rng, randomGen, grid, src_x, src_y, dest_x, dest_y);
        //game.player1->initialize(src_x, src_y, dest_x, dest_y);

        cout<<"EPOCH "<<episodeCount<<endl;
        if (train_step % dqnTargetUpdateNextEpisode == 0) {
            updateTargetNet();
        }
        game.learnToPlay(grid, enemies);
        train_step++;
        cout<<endl;
        printBoard(grid);
        cout<<"Total rewards collected "<<game.getTotalRewardsCollected()<<endl;
        rewards.push_back(game.getTotalRewardsCollected());
        episodes.push_back(episodeCount);
        reset(grid);
        game.reset(grid);
        decayEpsilon();
    }

    plotLosses();

    RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();
    StringReference *errorMessage = new StringReference();
    auto success = DrawScatterPlot(imageReference, 1000, 1000, &episodes, &rewards, errorMessage);
    if(success){
        vector<double> *pngdata = ConvertToPNG(imageReference->image);
        WriteToFile(pngdata, "/Users/debrajray/MyComputer/RL-A-STAR-THESIS/plot/episode_rewards.png");
        DeleteImage(imageReference->image);
    }else{
        cerr << "Error: ";
        for(wchar_t c : *errorMessage->string){
            wcerr << c;
        }
        cerr << endl;
    }

}

void player::playGame(vector<std::vector<int>> &grid, vector<enemy> &enemies, int src_x, int src_y, int dest_x, int dest_y, TestResult &result) {
    gameSimulation game(grid);
    game.player1 = this;
    cout<<"Source ("<<src_x<<", "<<src_y<<") Destination ("<<dest_x<<", "<<dest_y<<")"<<endl;
    this->initialize(src_x, src_y, dest_x, dest_y);
    game.play(grid, enemies);
    cout<<endl;
    printBoard(grid);
    result.final_x = game.player1->current_x;
    result.final_y = game.player1->current_y;
    result.destination_x = game.player1->destination_x;
    result.destination_y = game.player1->destination_y;
    result.total_rewards = game.player1->total_rewards;
    reset(grid);
    game.reset(grid);
}

void player::observe(observation &ob, std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies, bool isRedirect) {
    cout<<"player::observe"<<endl;
    ob.playerX = this->current_x;
    ob.playerY = this->current_y;
    // TODO: Add to input state tensor
    ob.playerLifeLeft = static_cast<float>(this->life_left);

    if (!isRedirect) {
        ob.locateTrajectoryAndDirection(fp, destination_x, destination_y);
    }

    if (ob.direction > 0) {
        ob.locateEnemies(enemies);
        ob.updateObstacleDistances(grid);
    }
}

void player::reset(std::vector<std::vector<int>> &grid) {
    current_x = source_x;
    current_y = source_y;
    life_left = MAX_LIFE;
}

int player::getDirection() {
    return fp->pathDirection(current_x, current_y);
}

void player::findPathToDestination(std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies, int src_x, int src_y, int dst_x, int dst_y) {
    cout<<"Find path to destination"<<endl;
    fp = std::make_shared<findPath>(grid, src_x, src_y, dst_x, dst_y);
    fp->populateEnemyObstacles(enemies);
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

void player::printBoard(std::vector<std::vector<int>> &grid) {
    cout<<"print board"<<endl;
    for (int row=0; row<GRID_SPAN; row++) {
        for (int col=0; col<GRID_SPAN; col++) {
            if(grid[row][col]<0) {
                cout<<grid[row][col]<<" ";
            } else {
                cout<<" "<<grid[row][col]<<" ";
            }
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
    current_x = source_x;
    current_y = source_y;
    total_rewards = 0;

}

void player::findNewRoute(vector<std::vector<int>> &grid, observation &ob, vector<enemy> &enemies, int src_x, int src_y, int dst_x,
                          int dst_y) {
    cout<<"Find new temporary route to destination"<<endl;
    fp_temp_reroute = std::make_shared<findPath>(grid, src_x, src_y, dst_x, dst_y);
    fp_temp_reroute->populateEnemyObstacles(enemies);
    fp_temp_reroute->findPathToDestination();
    ob.rerouteDistance = fp_temp_reroute->getDistanceToDestination();
}

int player::switchToNewRoute(observation &ob) {
    cout<<"switchToNewRoute"<<endl;
    if (ob.rerouteDistance < 1000) {
        cout<<"New distance "<<ob.rerouteDistance<<endl;
        // implies last action was re-route
        fp = fp_temp_reroute;
        return 0;
    }
    return -1;
}

int player::selectAction(observation& currentState) {
    return RLNN_Agent::selectAction(currentState, episodeCount, &isExploring);
}

void player::memorizeExperienceForReplay(observation &current, observation &next, int action, float reward, bool done) {
    if (isExploring) {
        RLNN_Agent::memorizeExperienceForReplay(current, next, action, reward, done);
    }
}

void player::learnWithDQN() {
    RLNN_Agent::learnWithDQN();
}



