#include "common.hpp"
#include "square.hpp"
#include "pieces.hpp"
#include "sides.hpp"
#include "position.hpp"

class Capture {
public:
    static const u8 NullCapture = 0xFF;
    static const u8 NoCapture = 0;

    Capture() : value_(NullCapture) {}

private:
    u8 value_;
};
