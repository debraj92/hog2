//
// Created by Debraj Ray on 2021-11-14.
//

#ifndef EXAMPLE_GAMECONSTANTS_H
#define EXAMPLE_GAMECONSTANTS_H

const int MAX_EPISODES = 2000;
const int SESSION_TIMEOUT = 20;

const int GRID_SPAN=12;
static const int MAX_STATES = 5000;

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

const int on_track = 10;
const int one_deviation_N = 11;
const int one_deviation_S = 12;
const int one_deviation_W = 13;
const int one_deviation_E = 14;

const int two_deviation_N = 21;
const int two_deviation_S = 22;
const int two_deviation_W = 23;
const int two_deviation_E = 24;

const int ACTION_FOLLOW = 1;
const int ACTION_DODGE_LEFT = 2;
const int ACTION_DODGE_DIAGONAL_LEFT = 3;
const int ACTION_DODGE_RIGHT = 4;
const int ACTION_DODGE_DIAGONAL_RIGHT = 5;
const int ACTION_STRAIGHT = 6;
const int ACTION_SPACE = 6;

const int REWARD_DEATH = -50;
const int REWARD_REACH = 50;
const int REWARD_ACTION_UNAVAILABLE = -30;
const int REWARD_TRACK_FOLLOW = -1;
const int REWARD_TRACK_ONE_DIV = -2;
const int REWARD_TRACK_TWO_DIV = -3;
const int REWARD_OFFTRACK = -4;

#endif //EXAMPLE_GAMECONSTANTS_H
