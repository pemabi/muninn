#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/bitboard.hpp"
#include "../src/common.hpp"

TEST_CASE("Bitboard overloaded operators")  {
    Bitboard bb = Bitboard(UINT64_C(0x04223FF888402038), UINT64_C(0x0000000000007010));
    Bitboard verticals = Bitboard(UINT64_C(0x556AB55AAD56AB55), UINT64_C(0x000000000002AB55));
    Bitboard checkerboard = Bitboard(UINT64_C(0x5555555555555555), UINT64_C(0x000000000002AAAA));

    SECTION("AND") {
        Bitboard expected1 = Bitboard(UINT64_C(0x0422355888402010), UINT64_C(0x0000000000002010));
        auto result1 = bb & verticals;
        REQUIRE( result1 == expected1 );
        Bitboard expected2 = Bitboard(UINT64_C(0x0400155000400010), UINT64_C(0x0000000000002000));
        auto result2 = bb & checkerboard;
        REQUIRE( result2 == expected2 );
        Bitboard expected3 = Bitboard(UINT64_C(0x5540155005540155), UINT64_C(0x000000000002AA00));
        auto result3 = checkerboard & verticals;
        REQUIRE( result3 == expected3 );
    }
}


TEST_CASE("Magics")   {
    for (int i = 0; i < 50; ++i)  {
      //generate random blocker configs


      //find the moves on the fly

      // calculate moves with magics

      // compare
    }

}
