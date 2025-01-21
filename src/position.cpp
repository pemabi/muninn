#include "position.hpp"

void print_position(const Position& pos) {
    // Unicode pieces
    const std::string DEFENDER = "♟";
    const std::string KING = "♔";
    const std::string ATTACKER = "♙";
    const std::string EMPTY = "·";

    // go through each bitboard, print piece on the square. Maybe check all piece bbs and if there are multiple pieces on the same square, generate an error
    //TODO: make a windows compatible one with 'A''D' and 'K'

    // check for overlap in bbs
    if (pos.occupied() == (pos.attackers() ^ pos.defenders() ^ pos.king())) {

        for (Rank r = Rank9; r >= Rank1; --r) {
            std::cout << r+1 << " ";
            for (File f = FileA; f < FileNum; ++f) {
                Square sq = index_square(f, r);
                if (pos.attackers().is_set(sq)) {
                    std::cout << ATTACKER << " ";
                }
                else if (pos.defenders().is_set(sq)) {
                    std::cout << DEFENDER << " ";
                }
                else if (pos.king().is_set(sq)) {  //could just use King index here
                    std::cout << KING << " ";
                }
                else {
                    std::cout << EMPTY << " ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "  a b c d e f g h i\n";
    }
    else {
        std::cout << "overlap in position Bitboards!\n";
    }
}
