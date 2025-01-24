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

    RandomGenerator rng;

    init_table();

    Position pos;
    //print_position(pos);

    pos.set(Position::StartFEN);
    print_position(pos);


#endif
    return 0;
}
