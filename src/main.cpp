#include "init.hpp"
#include "magics.hpp"

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
    RandomGenerator rng;

    init_table();

    Bitboard bb = all_zero_bb() | EDGE_MASK;
    bb.print_board();

#endif

    return 0;
}
