#include "init.hpp"
#include "magics.hpp"

#include "position.hpp"

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

#endif
    return 0;
}
