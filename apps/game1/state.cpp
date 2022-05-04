//
// Created by Debraj Ray on 2022-01-03.
//


#include "state.h"
#include <iostream>

using namespace std;

unsigned long state::getMaxQValueAction() {
    printPolicy();
    return argmax(policy)(0) + 1;
}

void state::updateActionQvalue(int action, double q_value) {
    int index = action - 1;
    policy(index) = q_value;
    printData();
    printPolicy();
}

double state::getQvalueOfAction(int action) {
    //cout<<"state::getQvalueOfAction"<<endl;
    //cout<<"action"<<action<<endl;
    //printPolicy();
    return policy.at(action - 1);
}

double state::getMaxProbability() {
    return amax(policy)(0);
}

void state::printPolicy() {
    cout<<"X,Y ="<<x<<", "<<y<<endl;
    cout<<"Policy Size"<<policy.size()<<endl;
    cout<<"ACTION_FOLLOW: "<<policy.at(ACTION_FOLLOW-1)<<endl;
    cout<<"ACTION_DODGE_LEFT: "<<policy.at(ACTION_DODGE_LEFT -1)<<endl;
    cout<<"ACTION_DODGE_DIAGONAL_LEFT: "<<policy.at(ACTION_DODGE_DIAGONAL_LEFT -1)<<endl;
    cout<<"ACTION_DODGE_RIGHT: "<<policy.at(ACTION_DODGE_RIGHT -1)<<endl;
    cout<<"ACTION_DODGE_DIAGONAL_RIGHT: "<<policy.at(ACTION_DODGE_DIAGONAL_RIGHT -1)<<endl;
    cout<<"ACTION_STRAIGHT: "<<policy.at(ACTION_STRAIGHT -1)<<endl;
    //std::cout<<policy<<"\n";
}




