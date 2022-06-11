//
// Created by Debraj Ray on 2021-11-14.
//

#ifndef EXAMPLE_GAMECONSTANTS_H
#define EXAMPLE_GAMECONSTANTS_H

//const int MAX_EPISODES = 10000; // 20000;
//const int SESSION_TIMEOUT = 50; // 80;
//const int GRID_SPAN=6;//12;

/**
 * Debug Params
 */
const int MAX_EPISODES = 500;//3000;   /// Must be greater than 8
const int SESSION_TIMEOUT = 20;

const int GRID_SPAN=6; /// >= 10 when running unit tests

static const int MAX_STATES = 20000;

/**
 *    NW  N  NE
 *       |
 *     W- -E
 *       |
 *  SW  S  SE
 */

const int N=1;
const int S=2;
const int E=3;
const int W=4;
const int NE=5;
const int NW=6;
const int SE=7;
const int SW=8;
const int VISION_RADIUS = 2;

const int N_Angle=0;
const int S_Angle=180;
const int E_Angle=90;
const int W_Angle=270;
const int NE_Angle=45;
const int NW_Angle=315;
const int SE_Angle=135;
const int SW_Angle=225;

const int on_track = 10;
const int lower_bound_one_deviation = 11;
const int upper_bound_one_deviation = 14;

const int one_deviation_N = 11;
const int one_deviation_S = 12;
const int one_deviation_W = 13;
const int one_deviation_E = 14;

const int lower_bound_two_deviation = 21;
const int upper_bound_two_deviation = 24;

const int two_deviation_N = 21;
const int two_deviation_S = 22;
const int two_deviation_W = 23;
const int two_deviation_E = 24;


 /*
const int ACTION_SWITCH = 4;//0;
const int ACTION_DODGE_LEFT = 3;//1;//0;//1;
const int ACTION_DODGE_DIAGONAL_LEFT = 0;//2;
const int ACTION_DODGE_RIGHT = 2;//3;
const int ACTION_DODGE_DIAGONAL_RIGHT = 1;//3;//4;
const int ACTION_STRAIGHT = 7;//5;
const int ACTION_REROUTE = 6;
const int ACTION_REDIRECT = 5;//7;
  */



const int ACTION_DODGE_DIAGONAL_LEFT = 0;
const int ACTION_DODGE_DIAGONAL_RIGHT = 1;
const int ACTION_DODGE_LEFT = 2;
const int ACTION_DODGE_RIGHT = 3;
const int ACTION_STRAIGHT = 4;

const int ACTION_SWITCH = 7;//0;
//const int ACTION_DODGE_LEFT = 3;//1;//0;//1;

//const int ACTION_DODGE_RIGHT = 2;//3;


const int ACTION_REROUTE = 6;
const int ACTION_REDIRECT = 5;//7;

const int ACTION_SPACE = 5;//7;//8;


// deprecated action
const int ACTION_FOLLOW = 8;



// REWARDS
const float REWARD_DEATH = -0.1;//-50;
const float REWARD_REROUTE = -0.1;//-15;
const float REWARD_REDIRECT = -0.1;//-10;

// new rewards approved
/*const int REWARD_REACH = 300;
const int REWARD_ACTION_UNAVAILABLE = -10;
const int REWARD_TRACK_FOLLOW = -1;
const int REWARD_TRACK_ONE_DIV = -4;
const int REWARD_TRACK_TWO_DIV = -8;
const int REWARD_OFFTRACK = -10;
 */

/*
const float REWARD_ACTION_UNAVAILABLE = -0.1 * REWARD_FACTOR;
const float REWARD_TRACK_FOLLOW = -0.01 * REWARD_FACTOR;
const float REWARD_TRACK_ONE_DIV = -0.04 * REWARD_FACTOR;
const float REWARD_TRACK_TWO_DIV = -0.08 * REWARD_FACTOR;
const float REWARD_OFFTRACK = -0.1 * REWARD_FACTOR;
*/
const float REWARD_FACTOR = 100;
const float REWARD_REACH =  1 * REWARD_FACTOR;
const float REWARD_ACTION_UNAVAILABLE = -0.1 * REWARD_FACTOR;
const float REWARD_TRACK_FOLLOW = -0.01 * REWARD_FACTOR;
const float REWARD_TRACK_ONE_DIV = -0.04 * REWARD_FACTOR;
const float REWARD_TRACK_TWO_DIV = -0.08 * REWARD_FACTOR;
const float REWARD_OFFTRACK = -0.1 * REWARD_FACTOR;


const int MAX_LIFE = 10;

const int MAX_DISTACE = 1000;

const int MAX_ABSTRACT_OBSERVATIONS = 22;//35;

const int RISK_DISTANCE_MAX_MAGNITUDE = 10; //would increase if FOV increases

const int MAX_CAPACITY_REPLAY_BUFFER = 10000;

const int MIN_BUFFERED_EXPERIENCE_FOR_LEARNING = MAX_EPISODES * SESSION_TIMEOUT * 0.02;

const int SWITCH_TO_EXPLOITATION_ONLY_PERCENT = 90;

namespace RTS {
    enum LOG_LEVEL {
        INFO,
        DEBUG,
        OFF
    };
}

#endif //EXAMPLE_GAMECONSTANTS_H
