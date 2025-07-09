# pragma once

#include "move.hpp"
#include "position.hpp"
#include "common.hpp"
#include "UCTSearch.hpp"

class Agent {
public:
    static Move random(const BoardHistory& bh);
    static Move greedy(const BoardHistory& bh);
    static Move rl_training(const BoardHistory& bh);
    static Move player(const BoardHistory& bh);
};


namespace gameWrapper {
  // add in agents here, separate agent namespace perhaps

void loop();

}
