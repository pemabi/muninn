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

        for (int i = 0; i < 5; ++i)  {
            //generate random blocker pattern
            u64 low = rng.random_u64_sparse();
            u64 high = rng.random_u64_sparse();

            low = low & UINT64_C(0x7FFFFFFFFFFFFFFF);
            high = high & UINT64_C(0x000000000003FFFF);

            Bitboard random_blocker = Bitboard(low, high);
            Square random_square = static_cast<Square>(rng.random_int(0, 80));

            //find the attacks on the fly
            Bitboard moves_calculated = find_attacks_otf(random_blocker, random_square);

            // calculate moves with magics
            Bitboard moves_lookup = get_attacked_squares(random_square, random_blocker);

            std::cout<<"SQUARE: "<<random_square<<std::endl;
            std::cout<<"BOARD"<<std::endl;
            random_blocker.print_board();
            std::cout<<"OTF MOVES:"<<std::endl;
            moves_calculated.print_board();
            std::cout<<"MAGIC MOVES:"<<std::endl;
            moves_lookup.print_board();
        }

#endif

    return 0;
}
