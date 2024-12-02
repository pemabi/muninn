#include "common.hpp"
#include "bitboard.hpp"


// Square masks
const Bitboard SquareMaskBB[SquareNum] = {
    Bitboard(UINT64_C(1) << 0,                     0),     // 0, SQA1
    Bitboard(UINT64_C(1) << 1,                     0),     // 1, SQB1
    Bitboard(UINT64_C(1) << 2,                     0),     // 2, SQC1
    Bitboard(UINT64_C(1) << 3,                     0),     // 3, SQD1
    Bitboard(UINT64_C(1) << 4,                     0),     // 4, SQE1
    Bitboard(UINT64_C(1) << 5,                     0),     // 5, SQF1
    Bitboard(UINT64_C(1) << 6,                     0),     // 6, SQG1
    Bitboard(UINT64_C(1) << 7,                     0),     // 7, SQH1
    Bitboard(UINT64_C(1) << 8,                     0),     // 8, SQI1
    Bitboard(UINT64_C(1) << 9,                     0),     // 9, SQA2
    Bitboard(UINT64_C(1) << 10,                    0),     // 10, SQB2
    Bitboard(UINT64_C(1) << 11,                    0),     // 11, SQC2
    Bitboard(UINT64_C(1) << 12,                    0),     // 12, SQD2
    Bitboard(UINT64_C(1) << 13,                    0),     // 13, SQE2
    Bitboard(UINT64_C(1) << 14,                    0),     // 14, SQF2
    Bitboard(UINT64_C(1) << 15,                    0),     // 15, SQG2
    Bitboard(UINT64_C(1) << 16,                    0),     // 16, SQH2
    Bitboard(UINT64_C(1) << 17,                    0),     // 17, SQI2
    Bitboard(UINT64_C(1) << 18,                    0),     // 18, SQA3
    Bitboard(UINT64_C(1) << 19,                    0),     // 19, SQB3
    Bitboard(UINT64_C(1) << 20,                    0),     // 20, SQC3
    Bitboard(UINT64_C(1) << 21,                    0),     // 21, SQD3
    Bitboard(UINT64_C(1) << 22,                    0),     // 22, SQE3
    Bitboard(UINT64_C(1) << 23,                    0),     // 23, SQF3
    Bitboard(UINT64_C(1) << 24,                    0),     // 24, SQG3
    Bitboard(UINT64_C(1) << 25,                    0),     // 25, SQH3
    Bitboard(UINT64_C(1) << 26,                    0),     // 26, SQI3
    Bitboard(UINT64_C(1) << 27,                    0),     // 27, SQA4
    Bitboard(UINT64_C(1) << 28,                    0),     // 28, SQB4
    Bitboard(UINT64_C(1) << 29,                    0),     // 29, SQC4
    Bitboard(UINT64_C(1) << 30,                    0),     // 30, SQD4
    Bitboard(UINT64_C(1) << 31,                    0),     // 31, SQE4
    Bitboard(UINT64_C(1) << 32,                    0),     // 32, SQF4
    Bitboard(UINT64_C(1) << 33,                    0),     // 33, SQG4
    Bitboard(UINT64_C(1) << 34,                    0),     // 34, SQH4
    Bitboard(UINT64_C(1) << 35,                    0),     // 35, SQI4
    Bitboard(UINT64_C(1) << 36,                    0),     // 36, SQA5
    Bitboard(UINT64_C(1) << 37,                    0),     // 37, SQB5
    Bitboard(UINT64_C(1) << 38,                    0),     // 38, SQC5
    Bitboard(UINT64_C(1) << 39,                    0),     // 39, SQD5
    Bitboard(UINT64_C(1) << 40,                    0),     // 40, SQE5
    Bitboard(UINT64_C(1) << 41,                    0),     // 41, SQF5
    Bitboard(UINT64_C(1) << 42,                    0),     // 42, SQG5
    Bitboard(UINT64_C(1) << 43,                    0),     // 43, SQH5
    Bitboard(UINT64_C(1) << 44,                    0),     // 44, SQI5
    Bitboard(UINT64_C(1) << 45,                    0),     // 45, SQA6
    Bitboard(UINT64_C(1) << 46,                    0),     // 46, SQB6
    Bitboard(UINT64_C(1) << 47,                    0),     // 47, SQC6
    Bitboard(UINT64_C(1) << 48,                    0),     // 48, SQD6
    Bitboard(UINT64_C(1) << 49,                    0),     // 49, SQE6
    Bitboard(UINT64_C(1) << 50,                    0),     // 50, SQF6
    Bitboard(UINT64_C(1) << 51,                    0),     // 51, SQG6
    Bitboard(UINT64_C(1) << 52,                    0),     // 52, SQH6
    Bitboard(UINT64_C(1) << 53,                    0),     // 53, SQI6
    Bitboard(UINT64_C(1) << 54,                    0),     // 54, SQA7
    Bitboard(UINT64_C(1) << 55,                    0),     // 55, SQB7
    Bitboard(UINT64_C(1) << 56,                    0),     // 56, SQC7
    Bitboard(UINT64_C(1) << 57,                    0),     // 57, SQD7
    Bitboard(UINT64_C(1) << 58,                    0),     // 58, SQE7
    Bitboard(UINT64_C(1) << 59,                    0),     // 59, SQF7
    Bitboard(UINT64_C(1) << 60,                    0),     // 60, SQG7
    Bitboard(UINT64_C(1) << 61,                    0),     // 61, SQH7
    Bitboard(UINT64_C(1) << 62,                    0),     // 62, SQI7
    Bitboard(                    0, UINT64_C(1) << 0),     // 63, SQA8
    Bitboard(                    0, UINT64_C(1) << 1),     // 64, SQB8
    Bitboard(                    0, UINT64_C(1) << 2),     // 65, SQC8
    Bitboard(                    0, UINT64_C(1) << 3),     // 66, SQD8
    Bitboard(                    0, UINT64_C(1) << 4),     // 67, SQE8
    Bitboard(                    0, UINT64_C(1) << 5),     // 68, SQF8
    Bitboard(                    0, UINT64_C(1) << 6),     // 69, SQG8
    Bitboard(                    0, UINT64_C(1) << 7),     // 70, SQH8
    Bitboard(                    0, UINT64_C(1) << 8),     // 71, SQI8
    Bitboard(                    0, UINT64_C(1) << 9),     // 72, SQA9
    Bitboard(                   0, UINT64_C(1) << 10),     // 73, SQB9
    Bitboard(                   0, UINT64_C(1) << 11),     // 74, SQC9
    Bitboard(                   0, UINT64_C(1) << 12),     // 75, SQD9
    Bitboard(                   0, UINT64_C(1) << 13),     // 76, SQE9
    Bitboard(                   0, UINT64_C(1) << 14),     // 77, SQF9
    Bitboard(                   0, UINT64_C(1) << 15),     // 78, SQG9
    Bitboard(                   0, UINT64_C(1) << 16),     // 79, SQH9
    Bitboard(                   0, UINT64_C(1) << 17),     // 80, SQI9
};


const Bitboard FileMask[FileNum] = {
    File1Mask, File2Mask, File3Mask, File4Mask, File5Mask, File6Mask, File7Mask, File8Mask, File9Mask
};

const Bitboard RankMask[RankNum] = {
    Rank1Mask, Rank2Mask, Rank3Mask, Rank4Mask, Rank5Mask, Rank6Mask, Rank7Mask, Rank8Mask, Rank9Mask
};
