#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/bitboard.hpp"
#include "../src/common.hpp"
#include "../src/init.hpp"
#include "../src/square.hpp"

TEST_CASE("Move Table Entries")   {

    RandomGenerator rng;

    init_table();

    for (int i = 0; i < 50; ++i)  {
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
