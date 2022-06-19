//
// Created by Debraj Ray on 2022-06-10.
//

#ifndef EXAMPLE_LOGGER_H
#define EXAMPLE_LOGGER_H

#include "gameConstants.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
using namespace RTS;

class Logger {

    ofstream logfile;
    const string LOG_FILE = "/Users/debrajray/MyComputer/RL-A-STAR-THESIS/log/log.txt";

    void printBoard_(std::vector<std::vector<int>> &grid);

public:

    LOG_LEVEL level;

    static LOG_LEVEL GLOBAL_LOG_LEVEL;

    Logger(LOG_LEVEL l) : level(l) {

    }

    void openLogFile();

    void closeLogFile();

    Logger* logDebug(string output);

    Logger* logInfo(string output);

    Logger* logDebug(double output);

    Logger* logInfo(double output);

    Logger* logToFileInfo(string output);

    Logger* logToFileInfo(double output);

    Logger* endLineInfoFile();

    Logger* endLineDebug();

    Logger* endLineInfo();

    Logger* printBoardDebug(std::vector<std::vector<int>> &grid);

    Logger* printBoardInfo(std::vector<std::vector<int>> &grid);

};


#endif //EXAMPLE_LOGGER_H
