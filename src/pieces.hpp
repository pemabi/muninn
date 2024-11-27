# pragma once

#include "sides.hpp"
#include <cassert>

enum PieceType {
    Occupied, Pawn, King, PieceTypeNum
};

enum Piece {
    Empty, Attacker, Defender, King
};

inline Side pieceToSide(const Piece p)  {
    assert(p != Empty);
    return static_cast<Side>(p >> 1);  // 0 if Attacker, 1 if Defender/King - returning side of piece
}
