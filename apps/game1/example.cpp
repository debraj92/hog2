//#include <torch/torch.h>

#include <iostream>
#include "gameEnv.h"
#include "AStar_.h"
#include "gameConstants.h"


void test1(){
    vector<vector<int>> grid;
    for (int i=0; i<GRID_SPAN; i++) {
        std::vector<int> row(GRID_SPAN, 0);
        grid.push_back(row);
    }
    AStar_ aStar(grid, 0, 0, 1, 2);
    bool found = aStar.findPathToDestination();
    assert(found);
}


int main() {
//torch::Tensor tensor = torch::rand({2, 3});
//std::cout << tensor << std::endl;

    GameEnv gameEnv;
    gameEnv.train(1);

    test1();
}
