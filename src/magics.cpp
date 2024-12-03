#include <iostream>
#include "common.hpp"
#include "square.hpp"
#include "bitboard.hpp"
#include "init.hpp"

static u64 find_move_magic(const Square square) {
    RandomGenerator rng;
    Bitboard move[1<<BLOCKER_BITS[square]];
    Bitboard used[1<<(64-BLOCKER_SHIFT_BITS[square])];  // used is bigger for the squares that refuse to index nicely
    u64 key[1<<BLOCKER_BITS[square]];
    Bitboard mask = build_blocker_mask(square);
    Bitboard subset = all_zero_bb();
    int index = 0;

    for (int i = 0; i < 1<<BLOCKER_BITS[square]; ++i)  {
        key[index] = subset.merge();
        move[index] = find_moves_otf(subset, square);
        index +=1;
        subset = carry_rippler_next(subset, mask);
    }

    for (int i=0; i < 100000000; ++i) {
        const u64 magic = rng.random_u64_sparse();
        bool fail = false;
        if (count1s((mask.merge() * magic) & 0xFFF0000000000000) < 6) {
            continue;
        }

        for (int j = 0; j < 1<<(64-BLOCKER_SHIFT_BITS[square]); ++j)  {
            used[j] = all_one_bb();
        }

        for (int j=0; !fail && j < (1 << BLOCKER_BITS[square]); ++j) {
            const u64 index = get_hash_value(key[j], magic, BLOCKER_SHIFT_BITS[square]);
            if (used[index] == all_one_bb())  {
                used[index] = move[j];
            }
            else if (used[index] != move[j])  {
                fail = true;
            }
        }
        if (!fail)  {
            return magic;
        }
    }

    std::cout << "/***Failed***/\t";
    return 0;
}

static u64 find_attack_magic(const Square square) {
    RandomGenerator rng;
    Bitboard attack[1<<ATTACKER_BITS[square]];
    Bitboard used[1<<(64-ATTACKER_SHIFT_BITS[square])];
    u64 key[1<<ATTACKER_BITS[square]];
    Bitboard mask = build_attacker_mask(square);
    Bitboard subset = all_zero_bb();
    int index = 0;

    for (int i = 0; i < 1 << ATTACKER_BITS[square]; ++i)  {
        key[index] = subset.merge();
        attack[index] = find_attacks_otf(subset, square);
        index +=1;
        subset = carry_rippler_next(subset, mask);
    }

    for (int i=0; i < 100000000; ++i) {
        const u64 magic = rng.random_u64_sparse();
        bool fail = false;
        if (count1s((mask.merge() * magic) & 0xFFF0000000000000) < 6) {
            continue;
        }

        for (int j = 0; j < 1 << (64 - ATTACKER_SHIFT_BITS[square]); ++j)  {
            used[j] = all_one_bb();
        }

        for (int j=0; !fail && j < (1 << ATTACKER_BITS[square]); ++j) {
            const u64 index = get_hash_value(key[j], magic, ATTACKER_SHIFT_BITS[square]);
            if (used[index] == all_one_bb())  {
                used[index] = attack[j];
            }
            else if (used[index] != attack[j])  {
                fail = true;
            }
        }
        if (!fail)  {
            return magic;
        }
    }

    std::cout << "/***Failed***/\t";
    return 0;
}

void find_all_move_magics() {
    u64 MAGIC[SquareNum];

    std::cout << "const u64 MAGIC[81] = {" <<std::endl;
    for (Square sq = SQA1; sq < SquareNum; ++sq)  {
        MAGIC[sq] = find_move_magic(sq);
        std::cout << "\tUINT64_C(0x" << std::hex << MAGIC[sq] << ")," << std::endl;
    }
    std::cout << "};\n" << std::endl;
}

void find_all_attack_magics() {
  u64 FOUND_MAGIC[SquareNum];

  std::cout << "const u64 ATTACK_MAGIC[81] = {" <<std::endl;
  for (Square sq = SQA1; sq < SquareNum; ++sq)  {
      FOUND_MAGIC[sq] = find_attack_magic(sq);
      std::cout << "\tUINT64_C(0x" << std::hex << FOUND_MAGIC[sq] << ")," << std::endl;
  }
  std::cout << "};\n" << std::endl;

}
