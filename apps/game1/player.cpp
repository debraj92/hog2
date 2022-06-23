//
// Created by Debraj Ray on 2021-12-30.
//

#include "player.h"

#include <memory>
#include "gameSimulation.h"
#include "plot/pbPlots.hpp"
#include "plot/supportLib.hpp"
#include "trainingMaps.h"

#include <random>
#include <iostream>

void player::takeDamage(int points) {
    life_left -= points;
}

void player::learnGame() {

    vector<enemy> enemies;
    vector<std::vector<int>> grid;
    createEmptyGrid(grid);
    trainingMaps train;

    vector<double> rewards;
    gameSimulation game(grid);
    game.player1 = this;
    vector<enemy> tempEnemies;
    int src_x, src_y, dest_x, dest_y;
    bool resumed;

    for(episodeCount = 1; episodeCount <= MAX_EPISODES; episodeCount++) {
        // pick a random source and destination
        resumed = isResuming();
        if (not resumed) {
            /// If resumed, then do not change previous episode's source and destination.
            train.generateNextMap(grid, enemies);
            //train.setSourceAndDestination(grid, src_x, src_y, dest_x, dest_y);
            train.setSourceAndDestinationRotating( src_x, src_y, dest_x, dest_y);

            /// If resumed, then do not change enemy positions from last episode
            /// else reset enemy positions to start of game
            tempEnemies = enemies;
        }
        game.player1->initialize(src_x, src_y, dest_x, dest_y);

        logger->logInfo("Episode ")->logInfo(episodeCount)->endLineInfo();

        if (episodeCount % dqnTargetUpdateNextEpisode == 0) {
            updateTargetNet();
        }

        game.learnToPlay(grid, tempEnemies);
        logger->printBoardDebug(grid);
        logger->logInfo("Total rewards collected ")->logInfo(game.getTotalRewardsCollected())->endLineInfo();

        if (not resumed) {
            // Consider reward if the episode was not a resume. If there are multiple resumes, then only the 1st reward is considered.
            rewards.push_back(game.getTotalRewardsCollected());
        }
        game.removeCharacters(grid);
        decayEpsilon();
    }

    // Save Model
    saveModel(DQN_MODEL_PATH);

    plotLosses();
    plotRewards(rewards);

}

void player::playGame(vector<std::vector<int>> &grid, vector<enemy> &enemies, int src_x, int src_y, int dest_x, int dest_y, TestResult &result) {
    gameSimulation game(grid);
    game.player1 = this;
    logger->logInfo("Source (" + to_string(src_x) +", " + to_string(src_y) + ") Destination (" + to_string(dest_x) +", " + to_string(dest_y) +")\n")
            ->endLineInfo();
    this->initialize(src_x, src_y, dest_x, dest_y);
    game.play(grid, enemies);
    logger->printBoardInfo(grid);
    result.final_x = game.player1->current_x;
    result.final_y = game.player1->current_y;
    result.destination_x = game.player1->destination_x;
    result.destination_y = game.player1->destination_y;
    result.total_rewards = game.player1->total_rewards;
    game.removeCharacters(grid);
}

void player::observe(observation &ob, std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies) {
    logger->logDebug("player::observe")->endLineDebug();
    ob.playerX = this->current_x;
    ob.playerY = this->current_y;
    ob.destinationX = this->destination_x;
    ob.destinationY = this->destination_y;
    // TODO: Add to input state tensor
    ob.playerLifeLeft = static_cast<float>(this->life_left);

    ob.locateTrajectoryAndDirection(fp);
    ob.locateRelativeTrajectory();
    ob.findDestination();

    if (ob.direction > 0) {
        ob.locateEnemies(enemies);
        ob.updateObstacleDistances(grid);
    }
}

void player::findPathToDestination(std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies, int src_x, int src_y, int dst_x, int dst_y) {
    logger->logDebug("Find path to destination")->endLineDebug();
    fp = std::make_shared<findPath>(grid, src_x, src_y, dst_x, dst_y);
    //fp->populateEnemyObstacles(enemies);
    fp->findPathToDestination();
}

void player::initialize(int src_x, int src_y, int dest_x, int dest_y) {

    if (isResuming()) {
        current_x = restoreCellX;
        current_y = restoreCellY;
        source_x = restoreCellX;
        source_y = restoreCellY;
        resumeCount++;
    } else {
        source_x = src_x;
        source_y = src_y;
        current_x = source_x;
        current_y = source_y;
        resumeCount = resumeCount == MAX_RESUME? 0 : resumeCount;
    }

    destination_x = dest_x;
    destination_y = dest_y;
    life_left = MAX_LIFE;
    total_rewards = 0;

}

int player::selectAction(observation& currentState) {
    return RLNN_Agent::selectAction(currentState, episodeCount, &isExploring);
}

void player::memorizeExperienceForReplay(observation &current, observation &next, int action, float reward, bool done) {
    RLNN_Agent::memorizeExperienceForReplay(current, next, action, reward, done, isExploring);
}

double player::learnWithDQN() {
    return RLNN_Agent::learnWithDQN();
}

bool player::recordRestoreLocation(std::vector<enemy> &enemies) {
    bool isRestoreLocationSet = false;
    restoreCellX = fp->visited_x_onpath;
    restoreCellY = fp->visited_y_onpath;
    int nextCellX = -1, nextCellY = -1;
    while(not isRestoreLocationSet) {
        fp->getNextPositionAfterGivenLocation(restoreCellX, restoreCellY, nextCellX, nextCellY);
        restoreCellX = nextCellX;
        restoreCellY = nextCellY;
        if (restoreCellX == destination_x and restoreCellY == destination_y) {
            // cannot be restored
            return false;
        }
        isRestoreLocationSet = true;
        for (enemy e: enemies) {
            if (e.current_x == restoreCellX and e.current_y == restoreCellY) {
                isRestoreLocationSet = false;
                break;
            }
        }
    }

    // restored
    return true;
}

void player::plotRewards(vector<double> &rewards) {

    vector<double> rewards_averaged;
    vector<double> episodes;
    int avg_window_size = rewards.size() / MAX_REWARD_POINTS_IN_PLOT;
    for(int i=0; i<rewards.size(); i+=avg_window_size) {
        double sum = 0;
        for(int j=i; j<i+avg_window_size and j<rewards.size(); j++) {
            sum += rewards[j];
        }
        rewards_averaged.push_back(sum/avg_window_size);
        episodes.push_back(i+1);
    }


    RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();
    StringReference *errorMessage = new StringReference();
    auto success = DrawScatterPlot(imageReference, 1000, 1000, &episodes, &rewards_averaged, errorMessage);
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

bool player::isResuming() {
    return (not stopLearning) and playerDiedInPreviousEpisode and resumeCount < MAX_RESUME;
}

void player::createEmptyGrid(vector<std::vector<int>> &grid) {

    // initialize an empty grid
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
}



