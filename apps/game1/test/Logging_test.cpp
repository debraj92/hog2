//
// Created by Debraj Ray on 2022-06-10.
//

#include <gtest/gtest.h>
#include "../Logger.h"


TEST(LogStrings, BasicAssertions) {
    using namespace RTS;
    Logger logger(LOG_LEVEL::INFO);
    logger.logInfo("1.This should be printed\n");
    logger.logDebug("This should NOT be printed\n");

    logger = Logger(LOG_LEVEL::DEBUG);
    logger.logInfo("2.This should be printed\n");
    logger.logDebug("3.This should be printed\n");

    logger = Logger(LOG_LEVEL::OFF);
    logger.logInfo("This should NOT be printed\n");
    logger.logDebug("This should NOT be printed\n");

    Logger::GLOBAL_LOG_LEVEL = LOG_LEVEL::DEBUG;

    logger = Logger(LOG_LEVEL::INFO);
    logger.logInfo("4.This should be printed\n");
    logger.logDebug("5.This should be printed\n");

    logger = Logger(LOG_LEVEL::DEBUG);
    logger.logInfo("6.This should be printed\n");
    logger.logDebug("7.This should be printed\n");

    logger = Logger(LOG_LEVEL::OFF);
    logger.logInfo("This should NOT be printed\n");
    logger.logDebug("This should NOT be printed\n");

    Logger::GLOBAL_LOG_LEVEL = LOG_LEVEL::OFF;

    logger = Logger(LOG_LEVEL::INFO);
    logger.logInfo("This should NOT be printed\n");
    logger.logDebug("This should NOT be printed\n");

    logger = Logger(LOG_LEVEL::DEBUG);
    logger.logInfo("This should NOT be printed\n");
    logger.logDebug("This should NOT be printed\n");

    logger = Logger(LOG_LEVEL::OFF);
    logger.logInfo("This should NOT be printed\n");
    logger.logDebug("This should NOT be printed\n");
}

TEST(LogStringsMultiple, BasicAssertions) {
    using namespace RTS;
    std::unique_ptr<Logger> logger = std::make_unique<Logger>(LOG_LEVEL::INFO);
    logger->logInfo("This")->logInfo(" ")->logInfo("is")->logInfo(" ")->logInfo("a ")->logInfo("test")->endLineInfo();
    logger->logInfo("Print this number ")->logInfo(44)->endLineInfo();
    logger->logInfo("Print this float number ")->logInfo(44.44)->endLineInfo();
}
