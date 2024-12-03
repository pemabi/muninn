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

const u64 MAGIC[81] = {
        UINT64_C(0x100310000412100),
        UINT64_C(0x4204400000000),
        UINT64_C(0x2280002100000),
        UINT64_C(0x104104080900080),
        UINT64_C(0x140002018204061),
        UINT64_C(0x26880040008008),
        UINT64_C(0x4080800190000),
        UINT64_C(0x1001080000a0000),
        UINT64_C(0x10100400e8d0900),
        UINT64_C(0xa00600001040000),
        UINT64_C(0x2080080000168280),
        UINT64_C(0x604223080a101001),
        UINT64_C(0x18010012a00004),
        UINT64_C(0x2000022220200000),
        UINT64_C(0x430002004b80418),
        UINT64_C(0x2903408008a08044),
        UINT64_C(0x708000340240200),
        UINT64_C(0x89008c0000007050),
        UINT64_C(0x8800051004021000),
        UINT64_C(0x4801020000448080),
        UINT64_C(0x420000100880),
        UINT64_C(0x2480091010d010),
        UINT64_C(0x1142040560003180),
        UINT64_C(0x210040004208192),
        UINT64_C(0x1150406208109000),
        UINT64_C(0x1100804000000004),
        UINT64_C(0x4000000b8400030),
        UINT64_C(0x4200100100004480),
        UINT64_C(0x1010220010002000),
        UINT64_C(0x2110000080280b00),
        UINT64_C(0x8020000006000811),
        UINT64_C(0x5000100400020),
        UINT64_C(0xa1200002802068),
        UINT64_C(0x100201001090),
        UINT64_C(0x800088c20006000),
        UINT64_C(0x1000800402808000),
        UINT64_C(0x310004002000005),
        UINT64_C(0x2011008008041850),
        UINT64_C(0x601480c00001180),
        UINT64_C(0x283d10411008a504),
        UINT64_C(0x4082000302200),
        UINT64_C(0x28008940010600),
        UINT64_C(0x4800000020000),
        UINT64_C(0x8004481000000281),
        UINT64_C(0x18089000404000),
        UINT64_C(0x808460e060a48504),
        UINT64_C(0x8042304610000),
        UINT64_C(0x4011200102063004),
        UINT64_C(0x100c001040020100),
        UINT64_C(0x200109820000c048),
        UINT64_C(0x1c3c104084002),
        UINT64_C(0x820041044c10000),
        UINT64_C(0xa00501810000),
        UINT64_C(0xa10000000000101),
        UINT64_C(0x1e1000000010442),
        UINT64_C(0x4012a0004002),
        UINT64_C(0xc00004081021004),
        UINT64_C(0x6008280840000820),
        UINT64_C(0x1000050803004900),
        UINT64_C(0x8100000101240120),
        UINT64_C(0x8001000015200890),
        UINT64_C(0x2604284400801410),
        UINT64_C(0x8000109082000001),
        UINT64_C(0x20420242000080),
        UINT64_C(0xa20400000040200),
        UINT64_C(0x1000609008800100),
        UINT64_C(0x1040001002b000),
        UINT64_C(0x8260100000009108),
        UINT64_C(0x2804000000000001),
        UINT64_C(0xa001600010184000),
        UINT64_C(0x430001040000),
        UINT64_C(0x426002404020000),
        UINT64_C(0x8400009204108),
        UINT64_C(0x840000184004a820),
        UINT64_C(0x890080040800),
        UINT64_C(0x40044200108400),
        UINT64_C(0xc8180122080000a1),
        UINT64_C(0xec2012006210002),
        UINT64_C(0x404000002a80004),
        UINT64_C(0x60229800900100),
        UINT64_C(0x4210c0020),
};

// represents number of potential blockers in blocker set for each square
const int BLOCKER_BITS[SquareNum] = {
    14, 13, 13, 13, 13, 13, 13, 13, 14,
    13, 12, 12, 12, 12, 12, 12, 12, 13,
    13, 12, 12, 12, 12, 12, 12, 12, 13,
    13, 12, 12, 12, 12, 12, 12, 12, 13,
    13, 12, 12, 12, 12, 12, 12, 12, 13,
    13, 12, 12, 12, 12, 12, 12, 12, 13,
    13, 12, 12, 12, 12, 12, 12, 12, 13,
    13, 12, 12, 12, 12, 12, 12, 12, 13,
    14, 13, 13, 13, 13, 13, 13, 13, 14
};

// represents bits we shift when calculating move index. Generally follows pattern 64 - BLOCKER_BITS[sq]
const int BLOCKER_SHIFT_BITS[SquareNum] = {
    50, 51, 51, 51, 51, 51, 51, 51, 50,
    51, 52, 52, 52, 52, 52, 52, 52, 50, // [17]: 51 -> 50 : Followed Apery's lead here for hard to find (impossible?) magics
    51, 52, 52, 52, 52, 52, 52, 52, 51,
    51, 52, 52, 52, 52, 52, 52, 52, 51,
    51, 52, 52, 52, 52, 52, 52, 52, 51,
    51, 52, 52, 52, 52, 52, 52, 52, 50, // [53]: 51 -> 50
    51, 52, 52, 52, 52, 52, 52, 52, 51,
    51, 52, 52, 52, 52, 52, 52, 52, 51,
    50, 51, 51, 51, 51, 51, 51, 51, 50
};

// represents number of potential allied pieces coordinating to create an attack at each square.
const int ATTACKER_BITS[SquareNum] = {
    2, 2, 3, 3, 3, 3, 3, 2, 2,
    2, 2, 3, 3, 3, 3, 3, 2, 2,
    3, 3, 4, 4, 4, 4, 4, 3, 3,
    3, 3, 4, 4, 4, 4, 4, 3, 3,
    3, 3, 4, 4, 4, 4, 4, 3, 3,
    3, 3, 4, 4, 4, 4, 4, 3, 3,
    3, 3, 4, 4, 4, 4, 4, 3, 3,
    2, 2, 3, 3, 3, 3, 3, 2, 2,
    2, 2, 3, 3, 3, 3, 3, 2, 2
};

// represents bits we shift when calculating index. Could replace this with 64 - ATTACKER_BITS[sq], but want to keep consistency with moves
const int ATTACKER_SHIFT_BITS[SquareNum] = {
    62, 62, 61, 61, 61, 61, 61, 62, 62,
    62, 62, 61, 61, 61, 61, 61, 62, 62,
    61, 61, 60, 60, 60, 60, 60, 61, 61,
    61, 61, 60, 60, 60, 60, 60, 61, 61,
    61, 61, 60, 60, 60, 60, 60, 61, 61,
    61, 61, 60, 60, 60, 60, 60, 61, 61,
    61, 61, 60, 60, 60, 60, 60, 61, 61,
    62, 62, 61, 61, 61, 61, 61, 62, 62,
    62, 62, 61, 61, 61, 61, 61, 62, 62
};

const Bitboard FileMask[FileNum] = {
    File1Mask, File2Mask, File3Mask, File4Mask, File5Mask, File6Mask, File7Mask, File8Mask, File9Mask
};

const Bitboard RankMask[RankNum] = {
    Rank1Mask, Rank2Mask, Rank3Mask, Rank4Mask, Rank5Mask, Rank6Mask, Rank7Mask, Rank8Mask, Rank9Mask
};

Bitboard BLOCKER_MASK[SquareNum];
Bitboard MOVE[512000];
int INDEX_OFFSET[SquareNum];

Bitboard ATTACKER_MASK[SquareNum];
Bitboard ATTACK[784];
int ATTACK_INDEX_OFFSET[SquareNum];
