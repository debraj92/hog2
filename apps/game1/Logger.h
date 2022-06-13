//
// Created by Debraj Ray on 2022-06-10.
//

#ifndef EXAMPLE_LOGGER_H
#define EXAMPLE_LOGGER_H

#include "gameConstants.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace RTS;

class Logger {

    void printBoard_(std::vector<std::vector<int>> &grid);

public:

    LOG_LEVEL level;

    static LOG_LEVEL GLOBAL_LOG_LEVEL;

    Logger(LOG_LEVEL l) : level(l) {
    }

    Logger* logDebug(string output);

    Logger* logInfo(string output);

    Logger* logDebug(double output);

    Logger* logInfo(double output);

    Logger* endLineDebug();

    Logger* endLineInfo();

    Logger* printBoardDebug(std::vector<std::vector<int>> &grid);

    Logger* printBoardInfo(std::vector<std::vector<int>> &grid);

};


#endif //EXAMPLE_LOGGER_H
