#include <iostream>
#include "bitboard.hpp"

int main() {
    std::cout << "ONES:\n";
    Bitboard bb = all_one_bb();
    bb.print_board();
    std::cout << "\n\nZEROS:\n";
    bb = all_zero_bb();
    bb.print_board();
    std::cout << "\n\nSTART POS:\n";
    bb = start_pos();
    bb.print_board();
}
