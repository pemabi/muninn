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
        UINT64_C(0xc0001102008300),
        UINT64_C(0x80021208210100),
        UINT64_C(0x40010122000840),
        UINT64_C(0x240008100142140),
        UINT64_C(0x40030002101060),
        UINT64_C(0xc0003024a00050),
        UINT64_C(0x80002180051808),
        UINT64_C(0x440041110000104),
        UINT64_C(0x40001204044886),
        UINT64_C(0x8080400204010200),
        UINT64_C(0xc000100400022840),
        UINT64_C(0x802ca00200040080),
        UINT64_C(0x408a200600004001),
        UINT64_C(0x8009600080040120),
        UINT64_C(0x280a00040002001),
        UINT64_C(0x200100800100002),
        UINT64_C(0x21601000100002),
        UINT64_C(0x1418300001204081),
        UINT64_C(0x4c0055000410300),
        UINT64_C(0xa000088800220040),
        UINT64_C(0x410001000a08001),
        UINT64_C(0x84092002080040),
        UINT64_C(0x802414c000310044),
        UINT64_C(0x1040015000200010),
        UINT64_C(0x100004009414008),
        UINT64_C(0x8840109000100002),
        UINT64_C(0x1340001000010506),
        UINT64_C(0x8009089008000100),
        UINT64_C(0x810000824000040),
        UINT64_C(0x2028001000a022),
        UINT64_C(0x28000310010692),
        UINT64_C(0x400030c68004001),
        UINT64_C(0x40210008008808),
        UINT64_C(0x8840008008008404),
        UINT64_C(0x88010843000020c),
        UINT64_C(0x40000808000082),
        UINT64_C(0x280c090800040001),
        UINT64_C(0x4000100400820040),
        UINT64_C(0x2040040000840080),
        UINT64_C(0x800809004800c0),
        UINT64_C(0x440110020640020),
        UINT64_C(0x980148903180010),
        UINT64_C(0xa80004880480098),
        UINT64_C(0x8040006000440014),
        UINT64_C(0x28c000480244000a),
        UINT64_C(0x40100001415200),
        UINT64_C(0x20040082000300),
        UINT64_C(0x1000080100400180),
        UINT64_C(0xc00020001000201),
        UINT64_C(0x20081010008004),
        UINT64_C(0x100010000200201),
        UINT64_C(0x4100010040000402),
        UINT64_C(0x80a8400020000201),
        UINT64_C(0x40002050803a02),
        UINT64_C(0x460003000809081),
        UINT64_C(0x410850001a000227),
        UINT64_C(0x2400a0a00010001),
        UINT64_C(0x108009004444001),
        UINT64_C(0x10240020080000a),
        UINT64_C(0xa102000426200201),
        UINT64_C(0x2680004040002102),
        UINT64_C(0x10801010404028),
        UINT64_C(0xa04c00802049004),
        UINT64_C(0x4040000308010100),
        UINT64_C(0xaa0100001000080),
        UINT64_C(0xc00a1001000080),
        UINT64_C(0x40800a2000490040),
        UINT64_C(0x880010210001020),
        UINT64_C(0x2100008900c08044),
        UINT64_C(0x2080044080080188),
        UINT64_C(0x2240001000080094),
        UINT64_C(0xa040005000420482),
        UINT64_C(0x8a200002084100),
        UINT64_C(0x604080010401004a),
        UINT64_C(0xd0004800004000a0),
        UINT64_C(0x8032200041000220),
        UINT64_C(0x4800200101109010),
        UINT64_C(0x38a00011200190),
        UINT64_C(0x1000200020000408),
        UINT64_C(0x1470200020000102),
        UINT64_C(0x1300a00014010046),
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

const u64 ATTACK_MAGIC[81] = {
        UINT64_C(0x5103a2c002000084),
        UINT64_C(0x80a10a801040040),
        UINT64_C(0x8202050208000800),
        UINT64_C(0x14000280cc000410),
        UINT64_C(0x250080880a80430a),
        UINT64_C(0x480012020000002),
        UINT64_C(0x82a045020004001),
        UINT64_C(0x8206244310600104),
        UINT64_C(0x1302a2c82420800),
        UINT64_C(0xc801220400000),
        UINT64_C(0x9808100503240410),
        UINT64_C(0x41201200012020),
        UINT64_C(0x405209e080204100),
        UINT64_C(0x8190808040400300),
        UINT64_C(0x58450124202004),
        UINT64_C(0x2424111004000),
        UINT64_C(0x401800120e40201),
        UINT64_C(0x42a1102288f00081),
        UINT64_C(0x8104142002400000),
        UINT64_C(0x4000614102918000),
        UINT64_C(0x400608905000000),
        UINT64_C(0x802024441000000),
        UINT64_C(0xc84001210b400850),
        UINT64_C(0x600010942c00808),
        UINT64_C(0x4040032808208800),
        UINT64_C(0x9041208080101882),
        UINT64_C(0x42003080081202),
        UINT64_C(0x401402a441022003),
        UINT64_C(0x130600084224040),
        UINT64_C(0x10002420008082),
        UINT64_C(0x10248c00c20a0),
        UINT64_C(0x240000a0081208),
        UINT64_C(0x400824060001050),
        UINT64_C(0x840025601000),
        UINT64_C(0x200200044804900),
        UINT64_C(0x10822502301004a8),
        UINT64_C(0x302240000818100),
        UINT64_C(0x3000801050581e0),
        UINT64_C(0x2120a3004809010),
        UINT64_C(0x800c020814488810),
        UINT64_C(0x2044201008414b0),
        UINT64_C(0x8000489a81020004),
        UINT64_C(0x1000141d842002),
        UINT64_C(0x2010108804),
        UINT64_C(0x2000046004090805),
        UINT64_C(0x4c00001001104020),
        UINT64_C(0x1000600801004845),
        UINT64_C(0x10201001090c0808),
        UINT64_C(0x9010121a062a100),
        UINT64_C(0x80e41048c002404),
        UINT64_C(0xc11120008000c180),
        UINT64_C(0x1804000602840a0),
        UINT64_C(0x1040092011202401),
        UINT64_C(0x4510000104c0420),
        UINT64_C(0x4441a82022008252),
        UINT64_C(0x9140100440c0411),
        UINT64_C(0x58082000904060),
        UINT64_C(0x4002220300841030),
        UINT64_C(0x2004104028468021),
        UINT64_C(0x2004420806110442),
        UINT64_C(0x20820820a080823),
        UINT64_C(0x48a4229002884030),
        UINT64_C(0x80001041e0098804),
        UINT64_C(0x1220240001040180),
        UINT64_C(0xa34011020060d00),
        UINT64_C(0x4200804820013080),
        UINT64_C(0x1402a00250044880),
        UINT64_C(0x2100420002811041),
        UINT64_C(0x1504400008ab1084),
        UINT64_C(0x251410804023000),
        UINT64_C(0x1e22890300020a50),
        UINT64_C(0x1101048046014c1),
        UINT64_C(0x400b102811202208),
        UINT64_C(0x2308014010080093),
        UINT64_C(0xc6240822e01022),
        UINT64_C(0xa0a110000100820),
        UINT64_C(0x1080230000000a),
        UINT64_C(0x1048290040481008),
        UINT64_C(0x564215040c82a288),
        UINT64_C(0x622a800508804b),
        UINT64_C(0x220837024020002),
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
