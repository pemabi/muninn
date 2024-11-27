#include <iostream>
#include "bitboard.hpp"

int main() {
    Bitboard bb = all_one_bb();
    bb.print_board();
}
