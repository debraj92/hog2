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
    trainingMaps train(false);

    gameSimulation game(grid);
    game.player1 = this;
    vector<enemy> tempEnemies;
    int src_x, src_y, dest_x, dest_y;
    bool resumed;
    float destinationCount = 0;
    float deathCount = 0;
    float inferenceCount = 0;

    /// Start training async
    stopLearning = false;
    epoch = 1;
    thread trainingRunner([this]{runTrainingAsync();});

    while (epoch <= MAX_EPISODES) {
        // pick a random source and destination
        resumed = isResuming();
        if (not resumed) {
            /// If resumed, then do not change previous episode's source and destination.
            train.generateNextMap(grid, enemies);
            train.setSourceAndDestination(grid, src_x, src_y, dest_x, dest_y);
            //train.setSourceAndDestinationRotating( src_x, src_y, dest_x, dest_y);
            //train.setSourceAndDestinationFixed( src_x, src_y, dest_x, dest_y);

            /// If resumed, then do not change enemy positions from last episode
            /// else reset enemy positions to start of game
            tempEnemies = enemies;
        }
        game.player1->initialize(src_x, src_y, dest_x, dest_y);

        game.learnToPlay(grid, tempEnemies);
        if (stopLearning) {
            inferenceCount++;
        }
        if(stopLearning and game.isDestinationReached()) {
            destinationCount++;
        }
        if(stopLearning and life_left <= 0) {
            deathCount++;
        }
        logger->printBoardDebug(grid);

        game.removeCharacters(grid);
        decayEpsilon(epoch);

        /// Wake up the reader to run training
        {
            // critical
            std::unique_lock<mutex> locker(safeTrainingExploration);
            if (stopLearning) {
                logger->logInfo("Total rewards collected ")->logInfo(game.getTotalRewardsCollected())->endLineInfo();
                rewards.push_back(game.getTotalRewardsCollected());
                epoch++;
                logger->logInfo("Epoch: ")->logInfo(epoch)->endLineInfo();
                locker.unlock();
                explorationOpportunity.notify_one();
            } else {
                if (not resumed) {
                    aggregated_rewards += game.getTotalRewardsCollected();
                    count_aggregation++;
                }
                if (explorationCount >= MIN_EXPLORATION_BEFORE_TRAINING) {
                    locker.unlock();
                    explorationOpportunity.notify_one();
                }
                explorationCount++;
            }
        }
    }

    trainingRunner.join();
    // Save Model
    saveModel(DQN_MODEL_PATH);

    plotLosses();
    plotRewards(rewards);

    logger->logInfo("Destination reach %")->logInfo(destinationCount * 100 / inferenceCount)->endLineInfo();
    logger->logInfo("Death %")->logInfo(deathCount * 100 / inferenceCount)->endLineInfo();

}

void player::playGame(vector<std::vector<int>> &gridSource, vector<enemy> &enemies, int src_x, int src_y, int dest_x, int dest_y, TestResult &result) {
    copyGrid(gridSource);
    gameSimulation game(grid);
    game.player1 = this;
    logger->logDebug("Source (" + to_string(src_x) +", " + to_string(src_y) + ") Destination (" + to_string(dest_x) +", " + to_string(dest_y) +")\n")
            ->endLineDebug();
    this->initialize(src_x, src_y, dest_x, dest_y);
    game.play(grid, enemies);
    result.final_x = game.player1->current_x;
    result.final_y = game.player1->current_y;
    result.destination_x = game.player1->destination_x;
    result.destination_y = game.player1->destination_y;
    result.total_rewards = game.player1->total_rewards;
    game.removeCharacters(grid);
    logger->logDebug("Total rewards collected ")->logDebug(game.getTotalRewardsCollected())->endLineDebug();
}

void player::observe(observation &ob, std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies, int lastAction, bool wasPreviousStateHotPursuit) {
    logger->logDebug("player::observe")->endLineDebug();

    ob.playerX = this->current_x;
    ob.playerY = this->current_y;
    ob.destinationX = this->destination_x;
    ob.destinationY = this->destination_y;
    ob.playerLifeLeft = static_cast<float>(this->life_left);

    if (isSimpleAstarPlayer and (current_x != destination_x or current_y != destination_y)) {
        if (not findPathToDestination(grid, enemies, current_x, current_y, destination_x, destination_y)) {
            logger->logInfo("ERROR: Player could not find path to destination")->endLineInfo();
        }
    }

    ob.locateTrajectoryAndDirection(fp);
    ob.findDestination(isTrainingMode and not stopLearning);
    ob.locateRelativeTrajectory();

    if (ob.direction > 0) {
        ob.locateEnemies(grid, enemies);
        ob.updateObstacleDistances(grid);
    }

    ob.recordFOVForCNN(cnnController, fp);
    ob.actionInPreviousState = wasPreviousStateHotPursuit ? lastAction : -1;

}

bool player::findPathToDestination(std::vector<std::vector<int>> &grid, std::vector<enemy>& enemies, int src_x, int src_y, int dst_x, int dst_y) {
    logger->logDebug("Find path to destination")->endLineDebug();
    if (isSimpleAstarPlayer) {
        std::vector<std::vector<int>> gridTemporary;
        std::copy(grid.begin(), grid.end(), back_inserter(gridTemporary));
        populateEnemyObstacles(gridTemporary, enemies);
        fp = std::make_shared<findPath>(gridTemporary, src_x, src_y, dst_x, dst_y);
    } else {
        fp = std::make_shared<findPath>(grid, src_x, src_y, dst_x, dst_y);
    }
    return fp->findPathToDestination();
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

int player::selectAction(const observation& currentState) {
    return RLNN_Agent::selectAction(currentState, epoch, &isExploring);
}

void player::memorizeExperienceForReplay(observation &current, observation &next, int action, float reward, bool done) {
    // Cannot avoid transition into pursuit by any strategy. Therefore, nothing to learn.
    auto transitionIntoHotPursuit = not current.isPlayerInHotPursuit and next.isPlayerInHotPursuit;
    if (not stopLearning and not next.isGoalInSight and not transitionIntoHotPursuit) {
        RLNN_Agent::memorizeExperienceForReplay(current, next, action, reward, done, isExploring);
    }
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
    int avg_window_size = max(static_cast<int>(rewards.size() / MAX_REWARD_POINTS_IN_PLOT), 1);
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
        WriteToFile(pngdata, "/Users/debrajray/MyComputer/RL-A-STAR-THESIS/plot2/episode_rewards.png");
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

void player::loadExistingModel() {
    RLNN_Agent::loadModel(DQN_MODEL_PATH);
}

void player::copyGrid(std::vector<std::vector<int>> &gridSource) {
    for (int i=0; i<GRID_SPAN; i++) {
        for (int j=0; j<GRID_SPAN; j++) {
            grid[i][j] = gridSource[i][j];
        }
    }
}


void player::runTrainingAsync() {

    for(; epoch <= MAX_EPISODES and not stopLearning; epoch++) {

        /// Let the writer thread run a few times before executing a training pass
        {
            // critical
            std::unique_lock<mutex> locker(safeTrainingExploration);
            explorationOpportunity.wait(locker);
            if (stopLearning) {
                break;
            }
            rewards.push_back(aggregated_rewards/count_aggregation);
            explorationCount = 0;
            aggregated_rewards = 0;
            count_aggregation = 0;
        }
        if (epoch % dqnTargetUpdateNextEpisode == 0) {
            float percent_complete = (static_cast <float>(epoch) * 100) / static_cast <float>(MAX_EPISODES);
            if (percent_complete < 80) {
                logger->logInfo("Target net updated at episode ")->logInfo(epoch)->endLineInfo();
                updateTargetNet();
            }
        }
        logger->logInfo("Epoch: ")->logInfo(epoch)->endLineInfo();
        auto loss = RLNN_Agent::learnWithDQN();
        logger->logInfo("Network Loss: ")->logInfo(loss)->endLineInfo();
    }
}

void player::enableBaseLinePlayer() {
    isSimpleAstarPlayer = true;
}

void player::populateEnemyObstacles(vector<std::vector<int>> &grid, vector<enemy> &enemies) {
    logger->logDebug("populateEnemyObstacles")->endLineDebug();
    for(const enemy& e: enemies) {
        if (max(abs(current_x - e.current_x), abs(current_y - e.current_y)) <= VISION_RADIUS) {
            grid[e.current_x][e.current_y] = -e.id;
        }
    }

}



