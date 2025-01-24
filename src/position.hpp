#pragma once

#include "pieces.hpp"
#include "common.hpp"
#include "bitboard.hpp"
#include "sides.hpp"

#include <stack>
#include <string>
#include <cassert>

class Position;

enum GameResult : int8_t  {   // no draws in the game - maybe add win type for more info?
    AttackerWin, DefenderWin, GameResultNum
};

struct StateInfo {
    //int plies_from_null;

    //Key board_key;

    //StateInfo* previous;

    //ChangedLists cl;

    //Key key() const { return board_key; }
};

// smart pointer to manage a double ended queue of state info
using StateListPtr = std::unique_ptr<std::deque<StateInfo>>;

/*
Should add in some kind of compressing technique here (Huffman is obvious, custom ANS?) for ML export. Position, eval, etc.
Consider however the tensor stack - last few positions will be stored. should these all be encoded together? Should i not bother with encoding?
 */

class Move;
struct Thread;
struct Searcher;    // Think I might start with implementing a naive agent

class Position  {
public:
    // representation of inital position
    // static const CodedPos

    static const char* StartFEN;

    Position() = default;

    Position(Bitboard att, Bitboard def, Bitboard k, Square kingIdx, int ply, Side side)
        : attackersBB(att), defendersBB(def), kingBB(k), kingIndex(kingIdx), gamePly(ply), sideToMove(side) { }

    // some logic to set position from coded pos
    Position& set(const std::string& fenStr);
    const std::string fen() const;

    Bitboard attackers() const;
    Bitboard defenders() const;
    Bitboard king() const;
    Bitboard occupied() const;

    Square king_index() const;

    Side side_to_move() const;
    int game_ply() const;
    // maybe add some repetition counters etc here

    void clear(); // reset position to all zeros. I do this all manually. Could trial a lower level approach with memset later.

private:
    // move piece, remove piece, etc
    void put_piece(PieceType pt, Square sq);

    // Data members
    Bitboard attackersBB;
    Bitboard defendersBB;
    Bitboard kingBB;
    //Square attackerIndexList[16];
    //Square defenderIndexList[8];
    Square kingIndex;
    int gamePly;
    Side sideToMove;
    //StateInfo* st;
};

inline Side Position::side_to_move() const {
    return sideToMove;
}

inline Bitboard Position::attackers() const {
    return attackersBB;
}

inline Bitboard Position::defenders() const {
    return defendersBB;
}

inline Bitboard Position::king() const {
   return kingBB;
}

inline Bitboard Position::occupied() const {
    return attackersBB | defendersBB | kingBB;
}

inline Square Position::king_index() const {
    return kingIndex;
}

inline int Position::game_ply() const {
    return gamePly;
}

inline void Position::put_piece(PieceType pt, Square sq) {

    assert(pt == Attacker || pt == Defender || pt == King);

    if (pt == Attacker) {
        attackersBB.set_bit(sq);
    }
    else if (pt == Defender) {
        defendersBB.set_bit(sq);
    }
    else if (pt == King) {
        kingBB.set_bit(sq);
        kingIndex = sq;
    }
}



// Printing the position un unicode. Used for debugging and demonstrating only.
// Judge whether it is worth storing a mailbox representation in position,which would make this easier / faster
void print_position(const Position& pos);
