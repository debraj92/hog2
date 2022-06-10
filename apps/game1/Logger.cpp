//
// Created by Debraj Ray on 2022-06-10.
//

#include <sstream>
#include <iomanip>
#include "Logger.h"
#include <vector>
#include <cstdio>

using namespace RTS;
using namespace std;

LOG_LEVEL Logger::GLOBAL_LOG_LEVEL = INFO;

Logger* Logger::logDebug(string output) {
    switch (GLOBAL_LOG_LEVEL) {
        case DEBUG:
            if (level != OFF) {
                cout<<output;
            }
            return this;
        case INFO:
            switch (level) {
                case DEBUG:
                    cout<<output;
                    return this;
                case INFO:
                case OFF:
                    return this;
            }
            return this;
        case OFF:
            return this;
    }
}

Logger* Logger::logInfo(string output) {
    switch (GLOBAL_LOG_LEVEL) {
        case DEBUG:
            if (level != OFF) {
                cout<<output;
            }
            return this;
        case INFO:
            switch (level) {
                case DEBUG:
                case INFO:
                    cout<<output;
                    return this;
                case OFF:
                    return this;
            }
            return this;
        case OFF:
            return this;
    }
}

Logger* Logger::logDebug(double output) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << output;
    return logDebug(ss.str());
}

Logger* Logger::logInfo(double output) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << output;
    return logInfo(ss.str());
}

Logger *Logger::endLineDebug() {
    return logDebug("\n");
}

Logger *Logger::endLineInfo() {
    return logInfo("\n");
}

void Logger::printBoard_DEBUG(std::vector<std::vector<int>> &grid) {
    cout<<"print board"<<endl;
    for (int row=0; row<grid.size(); row++) {
        for (int col=0; col<grid[row].size(); col++) {
            if(grid[row][col]<0) {
                cout<<grid[row][col]<<" ";
            } else {
                cout<<" "<<grid[row][col]<<" ";
            }
        }
        cout<<"\n";
    }
}

Logger *Logger::printBoard(vector<std::vector<int>> &grid) {
    switch (GLOBAL_LOG_LEVEL) {
        case DEBUG:
            if (level != OFF) {
                printBoard_DEBUG(grid);
            }
            return this;
        case INFO:
            switch (level) {
                case DEBUG:
                    printBoard_DEBUG(grid);
                    return this;
                case INFO:
                case OFF:
                    return this;
            }
            return this;
        case OFF:
            return this;
    }
}


