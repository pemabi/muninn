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
    const SquareDelta deltaArray[4] = {DeltaN, DeltaE, DeltaS, DeltaW};
    Bitboard result = all_zero_bb();

    for (SquareDelta delta: deltaArray) {
        if (is_valid_target(sq, sq + delta + delta))  { // if square two over is on board and on same rank or file
            result.set_bit(sq + delta + delta);
        }
    }
    return result;
}

// 'manually' calculate piece moves for a given start square and board config. Inlcudes throne as possible destination.
Bitboard find_moves_otf(Bitboard& occupied, Square square)  {
    const SquareDelta deltaArray[4] = {DeltaN, DeltaE, DeltaS, DeltaW};
    Bitboard result = all_zero_bb();

    for (SquareDelta delta: deltaArray) {
        for (Square sq = square + delta; is_valid_target(square, sq); sq += delta)  {
            result.set_bit(sq);
            if (occupied.is_set(sq))  { // could swap these two so that moves didnt include blocker. In my current implementation, I need to AND with occupied anyway, so I am keeping it like this for now to make validation simpler. Adding edge squares would double the size of lookup table - price seems steep
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
        Bitboard subset = all_zero_bb();
        for (int i = 0; i < (1 << BLOCKER_BITS[sq]); ++i) {
            MOVE[index_offset + get_hash_value(subset.merge(), MAGIC[sq], BLOCKER_SHIFT_BITS[sq])] = find_moves_otf(subset, sq);
            subset = carry_rippler_next(subset, BLOCKER_MASK[sq]);
        }
        index_offset += 1 << (64 - BLOCKER_SHIFT_BITS[sq]);
    }
}

void init_attacks() {
    int index_offset = 0;
    for (Square sq = SQA1; sq < SquareNum; ++sq) {
        ATTACKER_MASK[sq] = build_attacker_mask(sq);
        ATTACK_INDEX_OFFSET[sq] = index_offset;
        Bitboard subset = all_zero_bb();
        for (int i = 0; i < (1 << ATTACKER_BITS[sq]); ++i)  {
            ATTACK[index_offset + get_hash_value(subset.merge(), ATTACK_MAGIC[sq], ATTACKER_SHIFT_BITS[sq])] = find_attacks_otf(subset, sq);
            subset = carry_rippler_next(subset, ATTACKER_MASK[sq]);
        }
        index_offset += 1 << (64 - ATTACKER_SHIFT_BITS[sq]); // in all cases here, 64 - SHIFT_BITS is equal to ATTACKER_BITS - we don't have the hashing issues that required the discrepencies in MOVE. Keeping it the same as MOVE for simplicity - may create a standard template later.
    }
}

void init_table() {
    init_moves();
    init_attacks(); // similar approach to the moves: we will mask allies with sqs that support attacks, then use magic #s to generate indexes to attacked squares. We will be able to mask these with enemy pieces.
}
