//
// Created by Debraj Ray on 2022-05-30.
//

#include "RLNN_Agent.h"

#include <torch/torch.h>
#include <utility>
#include <vector>

#include <iostream>

using namespace std;
using namespace torch;


int RLNN_Agent::selectAction(observation currentState) {

    cout<<"RLNN_Agent::selectAction"<<endl;
    int action;
    double lower_bound = 0;
    double upper_bound = 1;
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;
    re.seed(std::chrono::system_clock::now().time_since_epoch().count());
    double random_double = unif(re);
    if (random_double < epsilon) {
        // random action
        std::random_device rd;
        std::mt19937 gen(rd());
        // TODO: Action SPACE must be from 0 to ACTION_SPACE-1
        std::uniform_int_distribution<> distri(0, ACTION_SPACE-1);
        action = distri(gen);
    } else {
        double observation_vector[MAX_ABSTRACT_OBSERVATIONS];
        currentState.flattenObservationToVector(observation_vector);
        auto options = torch::TensorOptions().dtype(torch::kFloat64).device(torch::kCUDA);
        Tensor stateTensor = torch::from_blob(observation_vector, {MAX_ABSTRACT_OBSERVATIONS}, options);
        {
            torch::NoGradGuard no_grad;
            auto actions = policyNet->forward(stateTensor);
            action = torch::argmax(actions).item<int>();
        }
    }

    return action;
}

void RLNN_Agent::learn() {
    cout<<"RLNN_Agent::learn"<<endl;
    // select n samples picked uniformly at random from the experience replay memory, such that n=batchsize
    memory.sampleBatch(batchSize);

     // states have dimension: batch_size X observation_feature_size
     // output would have dimensions: batch_size X action_space
     /**
      * The actual actions selected are present in tensor_actions.
      * We want to gather their corresponding predicted q values.
      * We know that the expected q value should be close to 1.
      * So we try to reduce the loss.
      *
      * Explanation of gather: https://jamesmccaffrey.wordpress.com/2021/01/18/an-example-of-the-pytorch-gather-function/
      *
      * TODO: Actions must start from 0. Otherwise DQN will fail
      *
      * For example, say tensor_actions has the following actions in the batch: [3, 1, 0, 5, 4]
      * It means the 3rd action was taken in 0th experience and so on.
      * We want to gather Q value of this action and so provide its value as index.
      */
     auto q_pred = policyNet->forward(memory.tensor_states).gather(1, memory.tensor_actions.view({-1, 1}));
     // targetNet is used for prediction of next state Q value to reduce instability of bootstrapping.
     auto q_target = targetNet->forward(memory.tensor_next_states).max().values();
     // for all terminal states in the batch, set target state q value to 0
     torch::TensorList done_list = torch::TensorList(memory.batch_dones);
     q_target.index(done_list) = 0;
     auto y = memory.tensor_rewards + (gamma * q_target);
     y = y.view({-1, 1});

     // calculate the loss as the mean-squared error of y and qpred
    policyNet->computeLossAndBackPropagate(y, q_pred);
}

void RLNN_Agent::loadModel(string &file) {
    policyNet->loadModel(file);
}

void RLNN_Agent::saveModel(string &file) {
    policyNet->saveModel(file);
}

void RLNN_Agent::updateTargetNet() {
    std::stringstream stream;
    policyNet->saveModel(stream);
    targetNet->loadModel(stream);
}

void RLNN_Agent::decayEpsilon() {
    epsilon = max(epsilon_min, epsilon * epsilon_decay);
}
