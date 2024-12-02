#include <iostream>
#include "bitboard.hpp"
#include "init.hpp"

int main() {
    std::cout << "BLOCKER MASKS:\n";

    for (Square sq = SQA1; sq < SquareNum; ++sq)  {
        Bitboard bb = build_blocker_mask(sq);
        bb.print_board();
    }
}
