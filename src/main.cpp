#include "init.hpp"
#include "magics.hpp"

#include "position.hpp"
#include "move.hpp"

int main() {
#ifdef FIND_MOVE_MAGICS
    find_all_move_magics();
#endif

#ifdef FIND_ATTACK_MAGICS
    find_all_attack_magics();
#endif

#ifdef DEBUG_MODE

#endif

#ifdef TEST_MODE
    std::cout<<"hello, world"<<"\n";

    RandomGenerator rng;

    init_table();

    Bitboard bb = all_zero_bb();
    bb.print_board();

    Position pos;
    print_position(pos);

    int counter = 1;
    MoveList moves(pos);
    for (const auto& move : moves) {
        std::cout<<"MOVE "<<counter<<"\n";
        std::cout<<"RAW MOVE: "<<move.value()<<"\n";
        std::cout<<"FROM: "<<move.from()<<"\n";
        std::cout<<"TO: "<<move.to()<<"\n";
        std::cout<<"-----------\n";
        counter+=1;
    }
    std::cout<<moves.size()<<" Moves\n";


#endif
    return 0;
}
