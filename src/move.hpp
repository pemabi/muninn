#include "common.hpp"
#include "square.hpp"
#include "pieces.hpp"
#include "position.hpp"

class Move {
public:
    Move() {}
    explicit Move(const u16 u) : value_(u) {}
    Move& operator = (const Move& m) {value_ = m.value_; return *this; }
    Move(const Move& m) { value_ = m.value_; }

    Square to() const { return static_cast<Square>((value() >> 0) & 0x7f); }

    u16 value() const { return value_; }

private:
    u16 value_;
}
