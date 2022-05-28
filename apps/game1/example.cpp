#include <torch/torch.h>

#include <iostream>
#include "gameEnv.h"
#include "AStar_.h"
#include "gameConstants.h"

int main() {
//torch::Tensor tensor = torch::rand({2, 3});
//std::cout << tensor << std::endl;

//std::cout << torch::cuda::is_available << std::endl;


    GameEnv gameEnv;
    gameEnv.createMap3();
    gameEnv.printMap();
    gameEnv.train();


}
