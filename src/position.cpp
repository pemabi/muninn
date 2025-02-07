#include "position.hpp"
#include "move.hpp"

#include <sstream>

const char* Position::StartFEN = "3AAA3/4A4/4D4/A3D3A/AADDKDDAA/A3D3A/4D4/4A4/3AAA3 d 1";

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

  occupiedBB = occupied_from_pieces();
  allDefendersBB = all_defenders_from_pieces();

  ss >> token;
  sideToMove = (token == 'a' ? Attackers : Defenders);
  ss >> std::skipws;
  int ply;
  ss >> ply;
  gamePly = ply;

  return *this;
}

void Position::clear() {
    attackerBB = all_zero_bb();
    defenderBB = all_zero_bb();
    kingBB = all_zero_bb();
    allDefendersBB = all_zero_bb();
    occupiedBB = all_zero_bb();

    kingIndex = SQ_NONE;
    gamePly = 0;
    sideToMove = Attackers;
}

// takes king index and returns an index for the attacker configuration necessary for king capture. Only called when certain that the king index is one of these 5 sqs
int compress_king_index(int idx) {
    switch(idx) {
        case 31: return 0;
        case 39: return 1;
        case 40: return 2;
        case 41: return 3;
        case 49: return 4;
        default: return -1; // Error case
    }
}

void Position::make_captures(Square to) {
    Bitboard attacked_squares, captures, ally_key;
    if (sideToMove == Defenders) {
        ally_key = allDefendersBB | THRONE_MASK;  // OR with throne mask because empty or full throne can act as an ally for capturing purposes
        attacked_squares = get_attacked_squares(to, ally_key);
        captures = attackerBB & attacked_squares;
        attackerBB ^= captures;
        occupiedBB ^= captures;
    }
    else {
        if (kingIndex != 40) {  // check if king is on the throne
            ally_key = attackerBB | THRONE_MASK;  // OR with throne mask if throne is empty and can act as an ally for capturing purposes
        }
        else {
            ally_key = attackerBB;
        }
        attacked_squares = get_attacked_squares(to, ally_key);
        captures = defenderBB & attacked_squares;  // to store which pieces have been captured.
        defenderBB ^= captures;
        if (attacked_squares & kingBB) {  // If overlap on King BB and attacked sqs, then make some further checks. Worry is potential for unnoticed floating bits outside of board area
            if (AROUND_THRONE_MASK & kingBB) {  // check if King is on a square with special capture requirements (on / adjacent to throne)
                Bitboard attack_pattern = king_attack_pattern[compress_king_index(kingIndex)];
                if (attack_pattern == (attack_pattern & ally_key)) {
                    captures ^= kingBB;  // add the king capture to captures so it is XORed out of allDefendersBB and occupied at end of sequence
                    kingBB = all_zero_bb();
                    kingIndex = SquareNum;
                    win = Attackers;  // do I want this to be -1 / 1 instead of 1 / 2?
                }
            }
            else {
                captures ^= kingBB;
                kingBB = all_zero_bb();
                kingIndex = SquareNum;
                win = Attackers;
            }
        }
        allDefendersBB ^= captures;
        occupiedBB ^= captures;
    }
    // could modify attack hashing so that I don't need all of the safety checks here - remove attacks where allies are in neighbouring sq
    // should really test this vs just rebuilding the occupied and allDefenders with & operation
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

    make_captures(to);

    sideToMove = ~sideToMove;
}

void print_position_bbs(const Position& pos) {
    std::cout<<"ATTACKERS"<<'\n';
    pos.attacker_bb().print_board();
    std::cout<<"DEFENDERS"<<'\n';
    pos.defender_bb().print_board();
    std::cout<<"KING\n";
    pos.king_bb().print_board();
    std::cout<<"ALL DEFENDERS\n";
    pos.all_defenders_bb().print_board();
    std::cout<<"OCCUPIED\n";
    pos.occupied_bb().print_board();
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
    if (pos.occupied_bb() == (pos.attacker_bb() ^ pos.defender_bb() ^ pos.king_bb())) {

        for (Rank r = Rank9; r >= Rank1; --r) {
            std::cout << r+1 << " ";
            for (File f = FileA; f < FileNum; ++f) {
                Square sq = index_square(f, r);
                if (pos.attacker_bb().is_set(sq)) {
                    std::cout << ATTACKER << " ";
                }
                else if (pos.defender_bb().is_set(sq)) {
                    std::cout << DEFENDER << " ";
                }
                else if (pos.king_bb().is_set(sq)) {  //could just use King index here
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
        std::cout << "overlap in position Bitboards on Square!\n";
        print_position_bbs(pos);
    }
}
