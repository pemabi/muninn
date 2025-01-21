#pragma once

#include <cassert>
#include <iostream>  // debugging


#include "overloadEnumOperators.hpp"
#include "common.hpp"
#include "sides.hpp"

// Squares, where Square A1 is index 0, B1 is 1 ... I9 9s 80.
// In this way, it can be conceptualised as files as columns, ranks as rows.

enum Square  {
    SQA1, SQB1, SQC1, SQD1, SQE1, SQF1, SQG1, SQH1, SQI1,
    SQA2, SQB2, SQC2, SQD2, SQE2, SQF2, SQG2, SQH2, SQI2,
    SQA3, SQB3, SQC3, SQD3, SQE3, SQF3, SQG3, SQH3, SQI3,
    SQA4, SQB4, SQC4, SQD4, SQE4, SQF4, SQG4, SQH4, SQI4,
    SQA5, SQB5, SQC5, SQD5, SQE5, SQF5, SQG5, SQH5, SQI5,
    SQA6, SQB6, SQC6, SQD6, SQE6, SQF6, SQG6, SQH6, SQI6,
    SQA7, SQB7, SQC7, SQD7, SQE7, SQF7, SQG7, SQH7, SQI7,
    SQA8, SQB8, SQC8, SQD8, SQE8, SQF8, SQG8, SQH8, SQI8,
    SQA9, SQB9, SQC9, SQD9, SQE9, SQF9, SQG9, SQH9, SQI9,
    SquareNum,  // = 81
    SQ_NONE = SquareNum,  // to indicate invalid square / unoccupied square that should be skipped in movegen
    CutoffSquareLower = SQI7,   // Last square on lower board (p_[0])
    CutoffSquareUpper = SQA8    // First square on upper board (p_[1])
};
OVERLOAD_ENUM_OPERATORS(Square);

enum File  {
    FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH, FileI,
    FileNum,
    FileDeltaE = -1, FileDeltaW = 1,  // for iterating offset when calcaulting moves / attacks on the fly
    FileBegin = 0
};
OVERLOAD_ENUM_OPERATORS(File);

enum Rank  {
    Rank1, Rank2, Rank3, Rank4, Rank5, Rank6, Rank7, Rank8, Rank9,
    RankNum,
    RankDeltaN = 1, RankDeltaS= -1,
    RankBegin = 0
};
OVERLOAD_ENUM_OPERATORS(Rank);

inline bool is_in_file(const File f) { return (0 <= f) && (f < FileNum); }
inline bool is_in_rank(const Rank r) { return (0 <= r) && (r < RankNum); }
inline bool is_in_square(const Square s) { return (0 <= s) && (s < SquareNum); }
inline bool is_in_square(const File f, const Rank r) { return is_in_file(f) && is_in_rank(r); }

inline constexpr Square index_square(const File f, const Rank r)  {
    return static_cast<Square>(static_cast<int>(r) * 9 + static_cast<int>(f));
}

const File SquareToFile[SquareNum] = {
    FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH, FileI,
    FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH, FileI,
    FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH, FileI,
    FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH, FileI,
    FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH, FileI,
    FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH, FileI,
    FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH, FileI,
    FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH, FileI,
    FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH, FileI
};

inline File sq_to_file(const Square s)  {
    assert(is_in_square(s));
    return SquareToFile[s];
}

const Rank SquareToRank[SquareNum] = {
    Rank1, Rank1, Rank1, Rank1, Rank1, Rank1, Rank1, Rank1, Rank1,
    Rank2, Rank2, Rank2, Rank2, Rank2, Rank2, Rank2, Rank2, Rank2,
    Rank3, Rank3, Rank3, Rank3, Rank3, Rank3, Rank3, Rank3, Rank3,
    Rank4, Rank4, Rank4, Rank4, Rank4, Rank4, Rank4, Rank4, Rank4,
    Rank5, Rank5, Rank5, Rank5, Rank5, Rank5, Rank5, Rank5, Rank5,
    Rank6, Rank6, Rank6, Rank6, Rank6, Rank6, Rank6, Rank6, Rank6,
    Rank7, Rank7, Rank7, Rank7, Rank7, Rank7, Rank7, Rank7, Rank7,
    Rank8, Rank8, Rank8, Rank8, Rank8, Rank8, Rank8, Rank8, Rank8,
    Rank9, Rank9, Rank9, Rank9, Rank9, Rank9, Rank9, Rank9, Rank9
};

inline Rank sq_to_rank(const Square s)  {
    assert(is_in_square(s));
    return SquareToRank[s];
}

inline bool is_same_rank(const Square sq1, const Square sq2) {
    assert(is_in_square(sq1));
    assert(is_in_square(sq2));
    return sq_to_rank(sq1) == sq_to_rank(sq2);
}

inline bool is_same_file(const Square sq1, const Square sq2)  {
    assert(is_in_square(sq1));
    assert(is_in_square(sq2));
    return sq_to_file(sq1) == sq_to_file(sq2);
}

// ORDER MATTERS (FROM, TO) - true if target square is valid, and on either rank or file
inline bool is_valid_target(const Square from, const Square to) {
    assert(is_in_square(from));
    return is_in_square(to) && (is_same_file(from, to) || is_same_rank(from, to));
}

enum SquareDelta {
    DeltaNothing = 0,
    DeltaN = -1, DeltaE = -9, DeltaS = 1, DeltaW = 9,
};
OVERLOAD_ENUM_OPERATORS(SquareDelta);

inline Square operator + (const Square lhs, const SquareDelta rhs) { return lhs + static_cast<Square>(rhs); }
inline void operator += (Square& lhs, const SquareDelta rhs) { lhs = lhs + static_cast<Square>(rhs); }
inline Square operator - (const Square lhs, const SquareDelta rhs) { return lhs - static_cast<Square>(rhs); }
inline void operator -= (Square& lhs, const SquareDelta rhs) { lhs = lhs - static_cast<Square>(rhs); }
