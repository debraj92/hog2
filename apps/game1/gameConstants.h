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
const int MAX_EPISODES = 2000;   /// Must be greater than 8
const int SESSION_TIMEOUT = 40;

const int GRID_SPAN=10; /// >= 10 when running unit tests [7 for gameSimulation test]

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
const int FOV_WIDTH = 2 * VISION_RADIUS + 1;


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


/// ACTIONS
const int ACTION_DODGE_DIAGONAL_LEFT = 0;
const int ACTION_DODGE_DIAGONAL_RIGHT = 1;
const int ACTION_STRAIGHT = 2;

const int ACTION_DODGE_RIGHT = 3;
const int ACTION_DODGE_LEFT = 4;

const int ACTION_SWITCH = 7;


const int ACTION_REROUTE = 6;
const int ACTION_REDIRECT = 5;//7;

const int ACTION_SPACE = 3;


// deprecated action
const int ACTION_FOLLOW = 8;



// REWARDS
/*
const float REWARD_FACTOR = 10;
const float REWARD_REACH =  0.2 * REWARD_FACTOR;
const float REWARD_ACTION_UNAVAILABLE = -0.5 * REWARD_FACTOR;
const float REWARD_TRACK_ONE_DIV = -0.1 * REWARD_FACTOR;
const float REWARD_TRACK_TWO_DIV = -0.1 * REWARD_FACTOR;
const float REWARD_DEATH = -3 * REWARD_FACTOR;
const float REWARD_OFFTRACK = -3 * REWARD_FACTOR;
 */
const float REWARD_FACTOR = 10;
const float REWARD_REACH =  1 * REWARD_FACTOR; //6
const float REWARD_ACTION_UNAVAILABLE = -0.5 * REWARD_FACTOR;
const float REWARD_TRACK_ONE_DIV = -0.5 * REWARD_FACTOR;
const float REWARD_TRACK_TWO_DIV = -0.5 * REWARD_FACTOR;
const float REWARD_DEATH = -4 * REWARD_FACTOR;
const float REWARD_OFFTRACK = -4 * REWARD_FACTOR;


const int MAX_LIFE = 10;

const int MAX_DISTANCE = 100;

const int MAX_ABSTRACT_OBSERVATIONS = 17;

const int MAX_CAPACITY_REPLAY_BUFFER = 10000;

const int MIN_BUFFERED_EXPERIENCE_FOR_LEARNING = 1000;

const int SWITCH_TO_EXPLOITATION_ONLY_PERCENT = 90;

const int MAX_RESUME = (GRID_SPAN * 3) / 2; // Approx average path size

const double MIN_EXPLOITATION_WINDOW_START_FOR_MEMORY = 0.5;

const int MAX_REWARD_POINTS_IN_PLOT = 500;

const int PLAYER_ID = 9;

const int MAX_CHANNELS_CNN = 2;//3; // enemies, path, obstacle, direction

/*
const int INPUT_SIZE = 161;//89;
const int HIDDEN_LAYER_1_SIZE = 100;//60;
const int HIDDEN_LAYER_2_SIZE = 60;//40;
*/


const int INPUT_SIZE = 33;//89;
const int HIDDEN_LAYER_1_SIZE = 22;//60;
const int HIDDEN_LAYER_2_SIZE = 16;//40;

/*
const int INPUT_SIZE = 17;
const int HIDDEN_LAYER_1_SIZE = 11;
const int HIDDEN_LAYER_2_SIZE = 9;
*/

namespace RTS {
    enum LOG_LEVEL {
        INFO,
        DEBUG,
        OFF
    };
}

#endif //EXAMPLE_GAMECONSTANTS_H
