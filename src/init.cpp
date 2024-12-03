#include "common.hpp"
#include "init.hpp"
#include "bitboard.hpp"

// returns Bitboard of blocker mask (potential blocker sqs - full set of rank and file for given square, EDGE EXCLUSIVE)
Bitboard build_blocker_mask(const Square sq) {
    Bitboard result = square_file_mask(sq) ^ square_rank_mask(sq);
    if (sq_to_file(sq) != FileI) result &= ~file_mask<FileI>();   // crops out the edge squares, unless origin square is on an edge
    if (sq_to_file(sq)!= FileA) result &= ~file_mask<FileA>();
    if (sq_to_rank(sq) != Rank9) result &= ~rank_mask<Rank9>();
    if (sq_to_rank(sq) != Rank1) result &= ~rank_mask<Rank1>();
    return result;
}

// for a given square, returns a Bitboard with set squares on positions that support attacks (2 squares over)
Bitboard build_attacker_mask(const Square sq) {
    const SquareDelta deltaArrayRank[2] = {DeltaE + DeltaE, DeltaW + DeltaW};
    const SquareDelta deltaArrayFile[2] = {DeltaN + DeltaN, DeltaS + DeltaS};
    Bitboard result = all_zero_bb();

    for (SquareDelta delta: deltaArrayRank) {
        Square target_square = sq + delta;
        if (is_in_square(target_square) && is_same_rank(target_square, sq)) {
            result.set_bit(target_square);
        }
    }
    return result;
}

// 'manually' calculate piece moves for a given start square and board config. Inlcudes throne as possible destination.
Bitboard find_moves_otf(Bitboard& occupied, Square square)  {
    const SquareDelta deltaArrayRank[2] = {DeltaE, DeltaW};
    const SquareDelta deltaArrayFile[2] = {DeltaN, DeltaS};
    Bitboard result = all_zero_bb();
    for (SquareDelta delta: deltaArrayRank) {
        for (Square sq = square + delta;
            is_in_square(sq) && is_same_rank(square, sq);    // to protect against wraparound cases
            sq += delta)
        {
          result.set_bit(sq);
          if (occupied.is_set(sq))  {
              break;
          }
        }
    }
    for (SquareDelta delta: deltaArrayFile) {
        for (Square sq = square + delta;
            is_in_square(sq) && is_same_file(square, sq);
            sq += delta)
        {
          result.set_bit(sq);
          if (occupied.is_set(sq))  {
              break;
          }
        }
    }

    return result;
}

// manually calculate attacked square for a given start square and allied board occupancy
Bitboard find_attacks_otf(Bitboard& occupied_allies, Square square)  {
    const SquareDelta deltaArray[4] = {DeltaN, DeltaE, DeltaS, DeltaW};
    Bitboard result = all_zero_bb();

    for (SquareDelta delta: deltaArray) {
        if (is_valid_target(square, square + delta + delta) && occupied_allies.is_set(square + delta + delta))  { // if square two over is occupied
            result.set_bit(square + delta);
        }
    }
    return result;
}


// Carry Rippler trick for finding subsets of a bitset. Returns next subset
Bitboard carry_rippler_next(Bitboard& subset, Bitboard& mask) {
    Bitboard next;
    u64 carry;

    next.set(0, (subset.p(0) - mask.p(0)) & mask.p(0));
    carry = (subset.p(0) - mask.p(0)) > subset.p(0);
    next.set(1, ((subset.p(1) - carry) - mask.p(1)) & mask.p(1));

    return next;
}

// build the move tables. All moves made to edge of board regardless of occupants on edge
void init_moves()  {
    int index_offset = 0; // for breaking up the indices by square
    for (Square sq = SQA1; sq < SquareNum; ++sq)  {
        BLOCKER_MASK[sq] = build_blocker_mask(sq);
        INDEX_OFFSET[sq] = index_offset;
        const int blocker_bits = BLOCKER_BITS[sq];
        Bitboard subset = all_zero_bb();
        for (int i = 0; i < (1 << blocker_bits); ++i) {
            Bitboard occupied = carry_rippler_next(subset, BLOCKER_MASK[sq]);
            MOVE[index_offset + get_hash_value(occupied.merge(), MAGIC[sq], BLOCKER_SHIFT_BITS[sq])] = find_moves_otf(occupied, sq);
        }
        index_offset += 1 << (64 - BLOCKER_SHIFT_BITS[sq]);
    }
}

void init_attacks() {
    int index_offset = 0;
    for (Square sq = SQA1; sq < SquareNum; ++sq) {
        ATTACKER_MASK[sq] = build_attacker_mask(sq);
        ATTACK_INDEX_OFFSET[sq] = index_offset;
        const int attacker_bits = ATTACKER_BITS[sq];
        Bitboard subset = all_zero_bb();
        for (int i = 0; i < (1 << attacker_bits); ++i)  {
            Bitboard occupied = carry_rippler_next(subset, ATTACKER_MASK[sq]);
            ATTACK[index_offset + get_hash_value(occupied.merge(), ATTACK_MAGIC[sq], ATTACKER_SHIFT_BITS[sq])] = find_attacks_otf(occupied, sq);
        }
        index_offset += 1 << (64 - ATTACKER_SHIFT_BITS[sq]); // in all cases here, 64 - SHIFT_BITS is equal to ATTACKER_BITS - we don't have the hashing issues that required the discrepencies in MOVE. Keeping it the same as MOVE for simplicity - may create a standard template later.
    }
}

void init_table() {
    init_moves();
    init_attacks(); // similar approach to the moves: we will mask allies with sqs that support attacks, then use magic #s to generate indexes to attacked squares. We will be able to mask these with enemy pieces.
}

//#if defined FIND_MAGIC

u64 find_magic(const Square square) {
    RandomGenerator rng;
    Bitboard occupied[1<<BLOCKER_BITS[square]];
    Bitboard attack[1<<BLOCKER_BITS[square]];
    Bitboard used[1<<(64-BLOCKER_SHIFT_BITS[square])];
    u64 key[1<<BLOCKER_BITS[square]];
    Bitboard mask = build_blocker_mask(square);
    Bitboard subset = all_zero_bb();
    int index = 0;

    while (true)  {
        key[index] = subset.merge();
        attack[index] = find_moves_otf(subset, square);
        index +=1;
        subset = carry_rippler_next(subset, mask);
        if (subset.p(0) == 0 && subset.p(1) == 0) {
            break;
        }
    }

    for (int i=0; i < 1000000; ++i) {
        const u64 magic = rng.random_u64_sparse();
        bool fail = false;
        if (count1s((mask.merge() * magic) & 0xFFF0000000000000) < 6) {
            continue;
        }

        for (int j = 0; j < 1<<(64-BLOCKER_SHIFT_BITS[square]); ++j)  {
            used[j] = all_one_bb();
        }

        for (int j=0; !fail && j < (1 << BLOCKER_BITS[square]); ++j) {
            const u64 index = get_hash_value(occupied[j].merge(), magic, BLOCKER_SHIFT_BITS[square]);
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

//#endif
