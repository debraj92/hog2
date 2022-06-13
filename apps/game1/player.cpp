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
    vector<enemy> tempEnemies;
    int src_x, src_y, dest_x, dest_y;
    int storyIndex = 0;

    for(episodeCount = 1; episodeCount < MAX_EPISODES; episodeCount++) {
        // pick a random source and destination

        if (not isResuming()) {
            /// If resumed, then do not change previous episode's source and destination.
            src_x = sources[storyIndex][0];
            src_y = sources[storyIndex][1];
            dest_x = destinations[storyIndex][0];
            dest_y = destinations[storyIndex][1];
            /// Rotate stories when a fresh episode starts with no resume.
            //storyIndex++;
        }

        //game.player1->initialize(sources[i%14][0], sources[i%14][1], destinations[i%14][0], destinations[i%14][1]);
        game.player1->initialize(src_x, src_y, dest_x, dest_y);
        //selectRandomSourceAndDestinationCoordinates(rng, randomGen, grid, src_x, src_y, dest_x, dest_y);
        //game.player1->initialize(src_x, src_y, dest_x, dest_y);

        logger->logInfo("Episode ")->logInfo(episodeCount)->endLineInfo();

        if (train_step % dqnTargetUpdateNextEpisode == 0) {
            updateTargetNet();
        }
        if (not resumed) {
            /// If resumed, then do not change enemy positions from last episode
            /// else reset enemy positions to start of game
            tempEnemies = enemies;
        }

        game.learnToPlay(grid, tempEnemies);
        train_step++;
        logger->printBoard(grid);
        logger->logInfo("Total rewards collected ")->logInfo(game.getTotalRewardsCollected())->endLineInfo();
        if (not resumed) {
            rewards.push_back(game.getTotalRewardsCollected());
            game.reset(grid);
        }

        decayEpsilon();
    }

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
    logger->printBoard(grid);
    result.final_x = game.player1->current_x;
    result.final_y = game.player1->current_y;
    result.destination_x = game.player1->destination_x;
    result.destination_y = game.player1->destination_y;
    result.total_rewards = game.player1->total_rewards;
    reset(grid);
    game.reset(grid);
}

void player::observe(observation &ob, std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies, bool isRedirect) {
    logger->logDebug("player::observe")->endLineDebug();
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
    logger->logDebug("Find path to destination")->endLineDebug();
    fp = std::make_shared<findPath>(grid, src_x, src_y, dst_x, dst_y);
    // TODO: Enable when enemy handling is perfect
    //fp->populateEnemyObstacles(enemies);
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

void player::initialize(int src_x, int src_y, int dest_x, int dest_y) {

    if (isResuming()) {
        current_x = deathCellX;
        current_y = deathCellY;
        source_x = deathCellX;
        source_y = deathCellY;
        resumeCount++;
        resumed = true;
    } else {
        source_x = src_x;
        source_y = src_y;
        current_x = source_x;
        current_y = source_y;
        resumeCount = resumeCount == MAX_RESUME? 0 : resumeCount;
        resumed = false;
    }

    destination_x = dest_x;
    destination_y = dest_y;
    life_left = MAX_LIFE;
    total_rewards = 0;

}

void player::findNewRoute(vector<std::vector<int>> &grid, observation &ob, vector<enemy> &enemies, int src_x, int src_y, int dst_x,
                          int dst_y) {
    logger->logDebug("Find new temporary route to destination")->endLineDebug();
    fp_temp_reroute = std::make_shared<findPath>(grid, src_x, src_y, dst_x, dst_y);
    fp_temp_reroute->populateEnemyObstacles(enemies);
    fp_temp_reroute->findPathToDestination();
    ob.rerouteDistance = fp_temp_reroute->getDistanceToDestination();
}

int player::switchToNewRoute(observation &ob) {
    logger->logDebug("switchToNewRoute")->endLineDebug();
    if (ob.rerouteDistance < 1000) {
        logger->logDebug("New distance ")->logDebug(ob.rerouteDistance)->endLineDebug();
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
    if (playerDiedInPreviousEpisode and deathCellX == current.playerX and deathCellY == current.playerY) {
        return;
    }
    RLNN_Agent::memorizeExperienceForReplay(current, next, action, reward, done, isExploring);
}

double player::learnWithDQN() {
    return RLNN_Agent::learnWithDQN();
}

void player::recordDeathLocation() {
    deathCellX = current_x;
    deathCellY = current_y;
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



