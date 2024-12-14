#include "bitboard.hpp"
#include "pieces.hpp"
#include "sides.hpp"

#include <cassert>
#include <deque>
#include <memory>

struct StateInfo {
    //Key materialKey;
    int castlingRights;
    int rule50;
    int pliesFromNull;
    Square epSquare;

};

typedef std::unique_ptr<std::deque<StateInfo>> StateListPtr;

class Position {
public:
  Position() {}

  // because I have so few options - I could do these all explicitly
  Bitboard bbOf(const PieceType pt) const { return byTypeBB_[pt]; }   // why the underscore after
  Bitboard bbOf(const Side s) const { return bySideBB_[c]; }
  Bitboard bbOf(const PieceType pt, const Side s) const { return bbOf(pt) & bbOf(s); }

  Bitboard occupiedBB() const { return bbOf(Occupied); }
  Bitboard emptyBB() const { return occupiedBB() ^ all_one_bb(); }

  Piece piece(const Square sq) const { return piece_[sq]; }
  // could add in something for captured pieces here... tracking etc for gui

  // bools for board state
  bool kingEscaped() { const return }





};
