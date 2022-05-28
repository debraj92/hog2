//
// Created by Debraj Ray on 2022-05-09.
//

#include <gtest/gtest.h>
#include <vector>
#include "../observation.h"

using namespace std;

// Diagonal path
TEST(ObservationRedirect, BasicAssertions) {

    vector<vector<int>> grid;
    for (int i=0; i<10; i++) {
        std::vector<int> row(10, 0);
        grid.push_back(row);
    }

    observation ob;
    ob.redirect(5, 2, 5, 7);
    assert(ob.direction == E);

    ob.redirect(5, 2, 1, 2);
    assert(ob.direction == N);

    ob.redirect(5, 5, 6, 3);
    assert(ob.direction == SW);

    ob.redirect(5, 5, 6, 0);
    assert(ob.direction == W);

    ob.redirect(5, 5, 2, 3);
    assert(ob.direction == NW);

    ob.redirect(5, 5, 0, 4);
    assert(ob.direction == N);

}
