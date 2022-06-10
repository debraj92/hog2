//
// Created by Debraj Ray on 2022-06-08.
//

#ifndef EXAMPLE_DQN_INTERFACE_H
#define EXAMPLE_DQN_INTERFACE_H

#include "../observation.h"

class DQN_interface {

public:
    DQN_interface(){}
    virtual ~DQN_interface(){}

    virtual void setTrainingMode(bool value) = 0;

    virtual int selectAction(observation& currentState, int episodeCount, bool *explore) = 0;

    virtual void learnWithDQN() = 0;

    virtual void memorizeExperienceForReplay(observation &current, observation &next, int action, int reward, bool done) = 0;

    /// provide file path
    virtual void saveModel(string &file) = 0;

    /// provide file path
    virtual void loadModel(string &file) = 0;

    virtual void updateTargetNet() = 0;

    virtual void decayEpsilon() = 0;

    virtual void printAction(int action) = 0;

    virtual void plotLosses() = 0;

};


#endif //EXAMPLE_DQN_INTERFACE_H
