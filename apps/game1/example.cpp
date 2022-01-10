//#include <torch/torch.h>

#include <iostream>
#include "gameEnv.h"

int main() {
//torch::Tensor tensor = torch::rand({2, 3});
//std::cout << tensor << std::endl;

    GameEnv gameEnv;
    gameEnv.train(1);
}
