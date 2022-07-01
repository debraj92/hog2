//
// Created by Debraj Ray on 2022-05-30.
//

#include "RLNN_Agent.h"

#include <torch/torch.h>
#include <utility>
#include <vector>
#include <testing.h>

#include <iostream>

using namespace std;
using namespace torch;

int RLNN_Agent::selectAction(observation &currentState, int episodeCount, bool *explore) {

    int action;
    *explore = isTrainingMode and isExplore(episodeCount);
    if (*explore) {
        // random action
        logger->logDebug("Selecting random action")->endLineDebug();
        std::uniform_int_distribution<> distri(0, ACTION_SPACE-1);
        std::default_random_engine re;
#ifdef TESTING
        re.seed(seedAction++);
#else
        re.seed(std::chrono::system_clock::now().time_since_epoch().count());
#endif
        action = distri(re);
    } else {
        logger->logDebug("Selecting max action")->endLineDebug();
        auto options = torch::TensorOptions().dtype(torch::kFloat32).device(torch::kCPU);


        float obstaclesFOV[1][FOV_WIDTH][FOV_WIDTH];
        float enemiesFOV[1][FOV_WIDTH][FOV_WIDTH];

        copy(&currentState.obstaclesFOV[0][0], &currentState.obstaclesFOV[0][0] + FOV_WIDTH * FOV_WIDTH, &obstaclesFOV[0][0][0]);
        copy(&currentState.enemiesFOV[0][0], &currentState.enemiesFOV[0][0] + FOV_WIDTH * FOV_WIDTH, &enemiesFOV[0][0][0]);
        /*
        //TODO: Current must be taken before observation actually changes when the environment is dynamic

        cnn.populateFOVChannels(currentState.playerX, currentState.playerY, currentState.direction,
                                obstaclesFOV[0], enemiesFOV[0]);
                                */
        auto tensor_obstacles = torch::from_blob(obstaclesFOV, {1, FOV_WIDTH, FOV_WIDTH}, options).unsqueeze(1).clone();
        auto tensor_enemies = torch::from_blob(enemiesFOV, {1, FOV_WIDTH, FOV_WIDTH}, options).unsqueeze(1).clone();

        auto tensor_fov_channels= torch::cat({tensor_obstacles, tensor_enemies}, 1).clone();

        float observation_vector[MAX_ABSTRACT_OBSERVATIONS] = {0};
        currentState.flattenObservationToVector(observation_vector);
        auto tensor_states = torch::zeros({1, MAX_ABSTRACT_OBSERVATIONS}, options);
        tensor_states.slice(0, 0, 1) = torch::from_blob(observation_vector, {MAX_ABSTRACT_OBSERVATIONS}, options);
        float maxQ = 0;
        {
            torch::NoGradGuard no_grad;
            auto actions = policyNet->forwardPass(tensor_fov_channels, tensor_states);
            //auto actions = policyNet->forwardPass(tensor_states);
            action = torch::argmax(actions).detach().item<int>();
            maxQ = torch::max(actions).detach().item<float>();
            cout<<"Q values at ("<<currentState.playerX<<","<<currentState.playerY<<") : "<<actions<<endl;
            cout<<"direction: "<<currentState.direction<<endl;
        }

        string key = to_string(currentState.playerX)+"_"+to_string(currentState.playerY)+"_"+to_string(currentState.direction)+"_"+
                     to_string(pow(2,4)*currentState.trajectory_left + pow(2,3)*currentState.trajectory_on_track +
                                 pow(2,2)*currentState.trajectory_front + pow(2,1)*currentState.trajectory_off_track +
                                 pow(2,0)*currentState.trajectory_right);
        auto pos = state_q_.find(key);
        if (pos != state_q_.end()) {
            //handle the error
            float max = pos->second.q_values[0];
            max = pos->second.q_values[1] > max ? pos->second.q_values[1]:max;
            max = pos->second.q_values[2] > max ? pos->second.q_values[2]:max;
            //assert(abs(max - maxQ)<=3);
            if(abs(max - maxQ)>=15) {
                cout<<"Major difference"<<endl;
                cout<<"Q_PRED for state "<<key<<" -> "<<pos->second.q_values<<endl;
                cout<<"FOV CHANNEL IN BUFFER:"<<endl;
                cout<<pos->second.fov_channel<<endl;
                cout<<endl;
                cout<<"FOV CHANNEL IN SELECT:"<<endl;
                cout<<tensor_fov_channels<<endl;
            }
        }
    }

    printAction(action);
    return action;
}

double RLNN_Agent::learnWithDQN() {
    logger->logDebug("RLNN_Agent::learn")->endLineDebug();
    // start learning after the replay buffer is partially filled
    if (memory.getBufferSize() < MIN_BUFFERED_EXPERIENCE_FOR_LEARNING) {
        logger->logDebug("Ignoring learning attempt due to insufficient samples")->endLineDebug();
        return 0;
    }
    if (stopLearning) {
        logger->logDebug("Ignoring learning attempt due to finished training")->endLineDebug();
        return 0;
    }
    // select n samples picked uniformly at random from the experience replay memory, such that n=batchsize
    memory.sampleBatch(batchSize);

    // states have dimension: batch_size X observation_feature_size
    // output would have dimensions: batch_size X action_space
    auto q_pred1 = policyNet->forwardPass(memory.tensor_fov_channels_current, memory.tensor_states);
    //auto q_pred1 = policyNet->forwardPass(memory.tensor_states);


    /*
    auto q_pred2 = q_pred1.clone();
    q_pred2 = q_pred2.contiguous();
    auto* ptr = (float*)q_pred2.data_ptr();
    int i=0;
    state_q.clear();
    for (int r = 0; r < q_pred2.sizes()[0]; ++r)
    {
        q.clear();
        for (int c = 0; c < q_pred2.sizes()[1]; ++c)
        {
            q.push_back(*ptr++);
        }
        state_q.insert({memory.state_rep[i++], q});
    }
     */

    /*
    auto* ptr1 = (float*)q_pred2.data_ptr();
    i=0;
    //print
    cout<<endl<<endl;
    for (int r = 0; r < q_pred2.sizes()[0]; ++r)
    {
        vector<float> vf;
        for (int c = 0; c < q_pred2.sizes()[1]; ++c)
        {
            vf.push_back(*ptr1++);
        }
        cout<<memory.state_rep[i++]<<" -> "<<vf<<endl;
    }
     */

    //cout<<state_q<<endl;
    //cout<<q_pred2<<endl;

    auto q_pred =  q_pred1.gather(1, memory.tensor_actions.view({-1, 1}));

    // targetNet is used for prediction of next state Q value to reduce instability of bootstrapping.
    auto q_target = get<0>(targetNet->forwardPass(memory.tensor_fov_channels_next, memory.tensor_next_states).max(1, true));
    //auto q_target = get<0>(targetNet->forwardPass(memory.tensor_next_states).max(1, true));

    // for all terminal states in the batch, set target state q value to 0
    q_target = alpha * gamma * q_target * (1 - memory.tensor_dones.view({-1, 1}));

    auto discounted_reward = alpha * memory.tensor_rewards.view({-1, 1}) + (1 - alpha) * q_pred;

    // calculate expected q value
    auto y = discounted_reward + q_target;

    // calculate the loss as the mean-squared error of y and qpred
    auto loss = policyNet->computeLossAndBackPropagate(y, q_pred);

    /*
    if (loss <= 1 and ((episodeNumber >= 5000) and (episodeNumber % 1000 == 0))) {
        for(int i=0; i<batchSize; i++) {
            cout<<memory.current_state_coordinates_selected[i]<<"->"<<memory.next_state_coordinates_selected[i]<<endl;
        }
        cout<<"Actions: "<<memory.tensor_actions.view({-1, 1})<<endl;
        cout<<"Rewards: "<<memory.tensor_rewards.view({-1, 1})<<endl;
        cout<<"Dones: "<<memory.tensor_dones.view({-1, 1})<<endl;
        cout<<"y "<<y<<endl;
        cout<<"qtarget "<<q_target<<endl;
        cout<<"q_pred "<<q_pred<<endl;
    }
     */


    {
        torch::NoGradGuard no_grad;
        auto q_pred2 = policyNet->forwardPass(memory.tensor_fov_channels_current, memory.tensor_states);
        //auto q_pred2 = policyNet->forwardPass(memory.tensor_states);

        q_pred2 = q_pred2.contiguous();
        auto *ptr = (float *) q_pred2.data_ptr();
        int i = 0;
        state_q_.clear();
        for (int r = 0; r < q_pred2.sizes()[0]; ++r) {
            q.clear();
            for (int c = 0; c < q_pred2.sizes()[1]; ++c) {
                q.push_back(*ptr++);
            }
            //state_q.insert({memory.state_rep_selected[i++], q});
            state_q_.insert({memory.state_rep_selected[i], {q, memory.tensor_fov_channels_current.select(0, i).clone()}});
            i++;
        }
    }

    return loss;
}
/**
 *
1 0 0 0 0 2^4*trajectory_left = 16
0 1 0 0 0 2^3*trajectory_on_track = 8
0 0 1 0 0 2^2*trajectory_front = 4
0 0 0 1 0 2^1*trajectory_off_track = 2
0 0 0 0 1 2^0*trajectory_right = 1
 */


void RLNN_Agent::loadModel(const string &file) {
    policyNet->loadModel(file);
}

void RLNN_Agent::saveModel(const string &file) {
    policyNet->saveModel(file);
}

void RLNN_Agent::updateTargetNet() {
    std::stringstream stream1, stream2, stream3;

    policyNet->saveModel(stream1, DQNNet::SEQUENTIAL);
    targetNet->loadModel(stream1, DQNNet::SEQUENTIAL);
    policyNet->saveModel(stream2, DQNNet::CNN1);
    targetNet->loadModel(stream2, DQNNet::CNN1);
    policyNet->saveModel(stream3, DQNNet::POOL1);
    targetNet->loadModel(stream3, DQNNet::POOL1);
}

void RLNN_Agent::decayEpsilon() {
    if (not startEpsilonDecay) {
        return;
    }
    epsilon = max(epsilon_min, epsilon * epsilon_decay);
}

void RLNN_Agent::memorizeExperienceForReplay(observation &current, observation &next, int action, float reward, bool done, bool isExploring) {
    memory.storeExperience(current, next, action, reward, done, isExploring);
}

void RLNN_Agent::printAction(int action) {
    logger->logDebug("RLNN_Agent::printAction ");
    switch(action) {
        case ACTION_DODGE_DIAGONAL_LEFT:
            logger->logDebug("ACTION_DODGE_DIAGONAL_LEFT")->endLineDebug();
            break;
        case ACTION_DODGE_DIAGONAL_RIGHT:
            logger->logDebug("ACTION_DODGE_DIAGONAL_RIGHT")->endLineDebug();
            break;
        case ACTION_STRAIGHT:
            logger->logDebug("ACTION_STRAIGHT")->endLineDebug();
            break;
        default:
            logger->logDebug("INVALID ACTION")->endLineDebug();
    }
}

void RLNN_Agent::plotLosses() {
    policyNet->plotLoss();
}

bool RLNN_Agent::isExplore(int episodeCount) {

    double episodeCompletion = static_cast<double> (episodeCount) / MAX_EPISODES * 100;
    if(episodeCompletion < epsilon_annealing_percent) {
        return true;
    }
    if (episodeCompletion > SWITCH_TO_EXPLOITATION_ONLY_PERCENT) {
        stopLearning = true;
        return false;
    }
    startEpsilonDecay = true;
    double lower_bound = 0;
    double upper_bound = 1;
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;
#ifdef TESTING
    re.seed(seedExplore++);
#else
    re.seed(std::chrono::system_clock::now().time_since_epoch().count());
#endif

    return unif(re) < epsilon;
}

void RLNN_Agent::setTrainingMode(bool value) {
    isTrainingMode = value;
    if (!isTrainingMode) {
        policyNet->eval();
    }
}
