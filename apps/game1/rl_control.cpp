//
// Created by Debraj Ray on 2022-01-03.
//

#include "rl_control.h"

state* rl_control::evaluateActionProbabilities(int reward, observation& next_observation, int current_action) {
    cout<<"rl_control::evaluateActionProbabilities"<<endl;
    int next_state_index = getStateFromObservation(next_observation);
    if (next_state_index == -1) {
        // this state is encountered for the first time
        createNonTerminalState(next_observation);
        next_state_index = nextFreeSlot - 1;
    }
    double qvalue = (1 - alpha) * getCurrentQValue(cur_state, current_action) + alpha * (reward + gamma * getMaxQ(&states[next_state_index]));
    cur_state->updateActionQvalue(current_action, qvalue);
    return &states[next_state_index];
}

double rl_control::getCurrentQValue(state *current_state, int current_action) {
    cout<<"rl_control::getCurrentQValue"<<endl;
    return current_state->getQvalueOfAction(current_action);
}

double rl_control::getMaxQ(state *given_state) {
    return given_state->getMaxProbability();
}

int rl_control::getStateFromObservation(observation &ob) {
    cout<<"rl_control::getStateFromObservation"<<endl;
    for(int i = 0; i<nextFreeSlot; i++) {
        state m = states[i];
        if(ob.enemy_distance == states[i].enemy_distance && ob.enemy_cosine == states[i].enemy_cosine &&
        ob.trajectory == states[i].trajectory && checkObstacles(ob, states[i])
        && ob.direction == states[i].direction) {
            return i;
        }
    }
    return -1;
}

void rl_control::createNonTerminalState(observation &ob) {
    cout<<"rl_control::createNonTerminalState"<<endl;
    state new_state;
    new_state.enemy_distance = ob.enemy_distance;
    new_state.enemy_cosine = ob.enemy_cosine;
    new_state.trajectory = ob.trajectory;
    new_state.direction = ob.direction;
    copyObstacleInfo(ob, new_state);
    states[nextFreeSlot++] = new_state;
    cout<<"created state "<<nextFreeSlot-1<<endl;
}


int rl_control::getNextAction() {
    cout<<"rl_control::getNextAction"<<endl;
    //random::seed(time(NULL));
    double random = random::rand<double>({1}, 0, 1).at(0);
    if (random < epsilon) {
        // random action
        return random::randint({1},1,6).at(0);
    } else {
        return cur_state->getMaxQValueAction();
    }
}

void rl_control::createStartState(observation& ob) {
    int next_state_index = getStateFromObservation(ob);
    if (next_state_index == -1) {
        state new_state;
        new_state.enemy_distance = ob.enemy_distance;
        new_state.enemy_cosine = ob.enemy_cosine;
        new_state.trajectory = ob.trajectory;
        new_state.direction = ob.direction;
        copyObstacleInfo(ob, new_state);
        states[nextFreeSlot++] = new_state;
        cur_state = &states[0];
        cout<<"created state 0"<<endl;
    }
}

bool rl_control::checkObstacles(observation &ob, state &s) {
    return ob.obstacle_front == s.obstacle_front && ob.obstacle_left == s.obstacle_left &&
    ob.obstacle_front_left == s.obstacle_front_left && ob.obstacle_front_right == s.obstacle_front_right
    && ob.obstacle_right == s.obstacle_right;
}

void rl_control::make_greedy() {
    epsilon *= epsilon_decay;
}

void rl_control::printAllStatesAndPolicies() {
    cout<<"Printing all states and policies"<<endl;
    for(int i=0; i<nextFreeSlot; i++) {
        cout<<"state "<<i<<endl;
        states[i].printData();
        states[i].printPolicy();
    }
}

void rl_control::copyObstacleInfo(observation &ob, state& new_state) {
    new_state.obstacle_front = ob.obstacle_front;
    new_state.obstacle_left = ob.obstacle_left;
    new_state.obstacle_front_left = ob.obstacle_front_left ;
    new_state.obstacle_right = ob.obstacle_right;
    new_state.obstacle_front_right = ob.obstacle_front_right;
}

