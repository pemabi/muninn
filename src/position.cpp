#include "position.hpp"
#include "move.hpp"

#include <sstream>

const char* Position::StartFEN = "3AAA3/4A4/4D4/A3D3A/AADDKDDAA/A3D3A/4D4/4A4/3AAA3 a 1";

Position& Position::set(const std::string& fenStr) {
  /*

      Sets Position object based on modified FEN.

      FEN anatomy:
      1) Split into ranks divided by '/' character
      2) A represents attackers, D represents defenders, K represents king.
      3) use run-length for empties where number of sequential empties in a row are represented numerically.
      4) blankspace, then a or d represents Attacker or Defender turn.

      Starting FEN: "3AAA3/4A4/4D4/A3D3A/AADDKDDAA/A3D3A/4D4/4A4/3AAA3 a 1"

  */
  unsigned char token;
  Square sq = SQA9;
  std::istringstream ss(fenStr);

  ss >> std::noskipws;

  clear();

  while ((ss >> token) && !isspace(token)) {
      if (isdigit(token)) {  // empties
          sq += (token - '0') * DeltaE;
      }

      else if (token == '/') {  // new rank
          sq += 2 * DeltaS; // typewriter reset to beginning of rank below
      }

      else if (token == 'A') {
          put_piece(Attacker, sq);
          ++sq;
      }

      else if (token == 'D') {
          put_piece(Defender, sq);
          ++sq;
      }

      else if (token == 'K') {
          put_piece(King, sq);  // put_piece will take care of updating kingIndex
          ++sq;
      }

      else {
          throw std::invalid_argument("Invalid character in FEN string " + fenStr + ": " + std::string(1, token));
      }
  }

  ss >> token;
  sideToMove = (token == 'a' ? Attackers : Defenders);
  ss >> std::skipws;
  int ply;
  ss >> ply;
  gamePly = ply;

  return *this;
}

void Position::clear() {
    attackersBB = all_zero_bb();
    defendersBB = all_zero_bb();
    kingBB = all_zero_bb();

    kingIndex = SQ_NONE;
    gamePly = 0;
    sideToMove = Attackers;
}

void Position::do_move(Move move) {
    assert(is_valid_move(move));

    //update zobrist key

    // handle state transfer

    // changes to Position
    ++gamePly;

    Side us = sideToMove;
    Side them = ~us;
    Square from = move.from();
    Square to = move.to();
    PieceType pt = move.movedPiece();

    assert(move.movedSide() == us);

    move_piece(pt, from, to);

    sideToMove = ~sideToMove;

}

void print_position(const Position& pos) {
    // Unicode pieces
    const std::string DEFENDER = "♟";
    const std::string KING = "♕";  // dark theme makes the white king annoying. Using the Queen unicode character instead.
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
