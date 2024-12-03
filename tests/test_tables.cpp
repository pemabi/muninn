#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/bitboard.hpp"
#include "../src/common.hpp"
#include "../src/init.hpp"
#include "../src/square.hpp"

TEST_CASE("Move Table Entries")   {

    RandomGenerator rng;

    init_table();

    for (int i = 0; i < 10000; ++i)  {
      //generate random blocker pattern
      u64 low = rng.random_u64_sparse();
      u64 high = rng.random_u64_sparse();

      low = low & UINT64_C(0x7FFFFFFFFFFFFFFF);
      high = high & UINT64_C(0x000000000003FFFF);

      Bitboard random_blocker = Bitboard(low, high);
      Square random_square = static_cast<Square>(rng.random_int(0, 80));

      //find the moves on the fly
      Bitboard moves_calculated = find_moves_otf(random_blocker, random_square);

      // calculate moves with magics
      Bitboard moves_lookup = get_moves_unmasked(random_square, random_blocker);

      if (moves_calculated != moves_lookup) {
          std::cout<<"BLOCKERS:"<<std::endl;
          random_blocker.print_board();
          std::cout<<"OTF MOVES:"<<std::endl;
          moves_calculated.print_board();
          std::cout<<"TABLE MOVES:"<<std::endl;
          moves_lookup.print_board();
      }

      REQUIRE(moves_calculated == moves_lookup);
    }

}

TEST_CASE("Attack Table Entries")   {

    RandomGenerator rng;

    init_table();

    for (int i = 0; i < 10000; ++i)  {
      //generate random blocker pattern
      u64 low = rng.random_u64_sparse();
      u64 high = rng.random_u64_sparse();

      low = low & UINT64_C(0x7FFFFFFFFFFFFFFF);
      high = high & UINT64_C(0x000000000003FFFF);

      Bitboard random_bb = Bitboard(low, high);
      Square random_square = static_cast<Square>(rng.random_int(0, 80));

      //find the moves on the fly
      Bitboard attacks_calculated = find_attacks_otf(random_bb, random_square);

      // calculate moves with magics
      Bitboard attacks_lookup = get_attacked_squares(random_square, random_bb);

      if (attacks_calculated != attacks_lookup) {
          Bitboard tmp = all_zero_bb();
          std::cout<<"SQUARE:"<<std::endl;
          tmp.set_bit(random_square);
          tmp.print_board();
          std::cout<<"OCCUPANCY:"<<std::endl;
          random_bb.print_board();
          std::cout<<"OTF ATTACKS:"<<std::endl;
          attacks_calculated.print_board();
          std::cout<<"TABLE ATTACKS:"<<std::endl;
          attacks_lookup.print_board();
      }

      REQUIRE(attacks_calculated == attacks_lookup);
    }
}
