# pragma once

#include "sides.hpp"
#include <cassert>

enum PieceType {
    Defender, King, Attacker, PieceNum
};
OVERLOAD_ENUM_OPERATORS(PieceType);

inline Side pieceToSide(const PieceType p)  {
    assert(p < PieceNum);
    return static_cast<Side>(p >> 1);  // 0 if Defender/King, 1 if Attacker - returning side of piece
}
