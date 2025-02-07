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
        : attackerBB(att), defenderBB(def), kingBB(k), kingIndex(kingIdx), gamePly(ply), sideToMove(side) { }

    // some logic to set position from coded pos
    Position& set(const std::string& fenStr);
    const std::string fen() const;  // TODO

    // access methods
    Bitboard attacker_bb() const;
    Bitboard defender_bb() const;
    Bitboard king_bb() const;
    Bitboard all_defenders_bb() const;
    Bitboard occupied_bb() const;
    Square king_index() const;
    Side side_to_move() const;
    int game_ply() const;
    Side winner() const;
    // maybe add some repetition counters etc here

    Bitboard all_defenders_from_pieces() const;
    Bitboard occupied_from_pieces() const;

    // moves on the position
    void do_move(Move move);

    void clear(); // reset position to all zeros. I do this all manually. Could trial a lower level approach with memset later.

private:
    // move piece, remove piece, etc
    void put_piece(PieceType pt, Square sq);
    void remove_piece(PieceType pt, Square sq);
    void move_piece(PieceType pt, Square from, Square to);
    void make_captures(Square to);

    // Data members
    Bitboard attackerBB;
    Bitboard defenderBB;
    Bitboard kingBB;

    Bitboard allDefendersBB;
    Bitboard occupiedBB;

    //Square attackerIndexList[16];
    //Square defenderIndexList[8];
    Square kingIndex;
    int gamePly;
    Side sideToMove;
    Side win = sideNum;
    //StateInfo* st;
};

inline Side Position::side_to_move() const {
    return sideToMove;
}

inline Bitboard Position::attacker_bb() const {
    return attackerBB;
}

inline Bitboard Position::defender_bb() const {
    return defenderBB;
}

inline Bitboard Position::king_bb() const {
   return kingBB;
}

inline Bitboard Position::all_defenders_bb() const {
    return allDefendersBB;
}

inline Bitboard Position::occupied_bb() const {
    return occupiedBB;
}

inline Square Position::king_index() const {
    return kingIndex;
}

inline int Position::game_ply() const {
    return gamePly;
}

inline Side Position::winner() const {
    return win;
}

inline Bitboard Position::all_defenders_from_pieces() const {
    return defenderBB | kingBB;
}

inline Bitboard Position::occupied_from_pieces() const {
    return attackerBB | defenderBB | kingBB;
}

inline void Position::put_piece(PieceType pt, Square sq) {

    assert(pt == Attacker || pt == Defender || pt == King);

    if (pt == Attacker) {
        attackerBB.set_bit(sq);
    }
    else if (pt == Defender) {
        defenderBB.set_bit(sq);
        allDefendersBB.set_bit(sq);
    }
    else if (pt == King) {
        kingBB.set_bit(sq);
        allDefendersBB.set_bit(sq);
        kingIndex = sq;
    }
    occupiedBB.set_bit(sq);
}

inline void Position::remove_piece(PieceType pt, Square sq) {  // will be useful for captures
    assert(pt == Attacker || pt == Defender || pt == King);
    assert(is_in_square(sq));

    if (pt == Attacker) {
        attackerBB ^= SquareMaskBB[sq];
    }
    else if (pt == Defender) {
        defenderBB ^= SquareMaskBB[sq];
        allDefendersBB ^= SquareMaskBB[sq];
    }
    else {
        kingBB ^= SquareMaskBB[sq];
        allDefendersBB ^= SquareMaskBB[sq];
        // could zero out King index here, or set it to Null SQ value? probably makes this more robust, but unnecessary / overkill imo
    }
    occupiedBB ^= SquareMaskBB[sq];
}

// responsible for updating the piece Bitboard, and king index if applicable
inline void Position::move_piece(PieceType pt, Square from, Square to) {
    assert(pt == Attacker || pt == Defender || pt == King);
    assert(is_in_square(from) && is_in_square(to));

    Bitboard moveBB = SquareMaskBB[from] ^ SquareMaskBB[to];

    if (pt == Attacker) {
        attackerBB ^= moveBB;
    }
    else if (pt == Defender) {
        defenderBB ^= moveBB;
        allDefendersBB ^= moveBB;
    }
    else {
        kingBB ^= moveBB;
        allDefendersBB ^= moveBB;
        kingIndex = to;
        if (moveBB & EDGE_MASK) {
            win = Defenders;
        }
    }
    occupiedBB ^= moveBB;
}

// Printing the position un unicode. Used for debugging and demonstrating only.
// Judge whether it is worth storing a mailbox representation in position,which would make this easier / faster
void print_position(const Position& pos);
