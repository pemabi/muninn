#include "common.hpp"
#include "square.hpp"
#include "pieces.hpp"
#include "sides.hpp"
#include "position.hpp"

// xxxxxxxx x1111111 From square = bits 0-6
// xx111111 1xxxxxxx To square = bits 7-14
// 11xxxxxx xxxxxxxx Piece = bits 15-16 - functionally means that 1xxxxxxx xxxxxxxx operates as Turn flag.

class Move {
public:
    static const u16 MoveNone = 0;

    Move() {}
    explicit Move(const u16 u) : value_(u) {}
    Move& operator = (const Move& m) {value_ = m.value_; return *this; }
    Move(const Move& m) { value_ = m.value_; }

    Square from() const { return static_cast<Square>((value() >> 0) & 0x7f); }
    Square to() const { return static_cast<Square>((value() >> 7) & 0x7f); }
    Piece movedPiece() const { return static_cast<Piece>((value() >> 14)); }
    Side movedSide() const { return static_cast<Side>((value() >> 15)); }

    explicit operator bool() const { return value() != MoveNone; }
    u16 value() const { return value_; }
    Move operator |= (const Move rhs) {
        this->value_ |= rhs.value();
        return *this;
    }
    Move operator | (const Move rhs) const { return Move(*this) |= rhs; }

private:
    u16 value_;
};

inline Move moveNone() { return static_cast<Move>(Move::MoveNone); }

inline Move fromSqToMove(const Square from) {return static_cast<Move>(from << 0); }
inline Move toSqToMove(const Square to) {return static_cast<Move>(to << 7); }
inline Move pieceTypeToMove(const PieceType pt) {return static_cast<Move>(pt << 15); }

inline Move buildMove(const PieceType pt, const Square from, const Square to) {
    return fromSqToMove(from) | toSqToMove(to) | pieceTypeToMove(pt);
}

// todo: add struct that bundles move with other data (move score, captured pieces?)
