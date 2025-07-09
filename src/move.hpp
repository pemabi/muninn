#pragma once

#include <string>

#include "common.hpp"
#include "square.hpp"
#include "pieces.hpp"
#include "sides.hpp"
#include "bitboard.hpp"

class Position; // forward declaration to avoid circular dependency issues

// xxxxxxxx x1111111 From square = bits 0-6
// xx111111 1xxxxxxx To square = bits 7-14
// 11xxxxxx xxxxxxxx PieceType = bits 15-16 - functionally means that 1xxxxxxx xxxxxxxx operates as Turn flag.

class Move {
public:
    static const u16 MoveNone = 0;

    Move() : value_(MoveNone) {}  // default initialises to MoveNone
    explicit Move(const u16 u) : value_(u) {}
    Move& operator = (const Move& m) { value_ = m.value_; return *this; }
    Move(const Move& m) { value_ = m.value_; }

    Square from() const { return static_cast<Square>((value() >> 0) & 0x7f); }
    Square to() const { return static_cast<Square>((value() >> 7) & 0x7f); }
    PieceType movedPiece() const { return static_cast<PieceType>((value() >> 14)); }
    Side movedSide() const { return static_cast<Side>((value() >> 15)); }

    explicit operator bool() const { return value() != MoveNone; }
    u16 value() const { return value_; }
    Move& operator |= (const Move rhs) { // changed this to return reference
        this->value_ |= rhs.value();
        return *this;
    }
    Move operator | (const Move rhs) const { return Move(*this) |= rhs; }

    bool operator == (const Move& m) const {return value_ == m.value_; }
    bool operator != (const Move& m) const {return value_ != m.value_; }

private:
    u16 value_;
};

inline Move moveNone() { return static_cast<Move>(Move::MoveNone); }

inline Move fromSqToMove(const Square from) {return static_cast<Move>(from << 0); }
inline Move toSqToMove(const Square to) {return static_cast<Move>(to << 7); }
inline Move pieceTypeToMove(const PieceType pt) {return static_cast<Move>(pt << 14); }

inline Move encodeMove(const PieceType pt, const Square from, const Square to) {
    return fromSqToMove(from) | toSqToMove(to) | pieceTypeToMove(pt);
}

bool is_valid_move(Move move);

// specialise std::hash for Move class - used to map to NN output space
template<>
struct std::hash<Move> {
    std::size_t operator()(const Move& move) const noexcept {
        return std::hash<std::uint16_t>{}(move.value());
    }
};

// todo: add struct that bundles move with other data (move score, captured pieces?)

// returns pointer to the end of the move list
Move* generate(const Position& pos, Move* moveList, Bitboard* allToSquares);

struct MoveList {
    explicit MoveList(const Position& pos) : last(generate(pos, moveList, &allToSquares)) {}

    const Move* begin() const { return moveList; }
    const Move* end() const { return last; }

    size_t size() const { return last - moveList; }
    bool empty() const { return size() == 0; }

    const Move get_move_by_index(int i) {
        if (i < size()) {
            return moveList[i];
        }
        else {
            std::cout<<"invalid move reached\n";
            return moveNone();
        }
    }

    Bitboard all_to_squares() const { return allToSquares; }

    bool contains(Move& move) const {
        for (const Move* it = begin(); it != end(); ++it) {
            if (*it == move) {
                return true;
            }
        }

        return false;
    }

private:
    Bitboard allToSquares;  // how I would like to structure this
    Move moveList[MAX_MOVES], *last;
};
