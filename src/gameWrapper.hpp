# pragma once

#include "move.hpp"
#include "position.hpp"
#include "common.hpp"
#include "UCTSearch.hpp"

class Agent {
public:
    static Move random(const BoardHistory& bh) {
        MoveList moves(bh.current_pos());
        if (moves.empty()) {
            std::cout<<"no move found\n";  // this case should only happen if intial position has no moves
            return moveNone();
        }
        Move move = moves.get_move_by_index(RandomGenerator::random_int(0, moves.size() - 1));
        return move;
    }

    static Move greedy() {return moveNone();}  // can add simple loop that iterates thru movelist, makes moves, counts bits / looks at captures / checks win

    static Move rl_training(const BoardHistory& bh) {
        // no transposition tables, etc, so think that stack allocation would be fine, but copying this approach
        // important to create new search between moves because tree reuse here can affect noise
        auto uct = std::make_unique<UCTSearch>(bh.shallow_clone());
        return uct->search(bh.shallow_clone());
    }
};

namespace gameWrapper {
  // add in agents here, separate agent namespace perhaps

void loop();

}
