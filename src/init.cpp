#include "common.hpp"
#include "init.hpp"

// returns Bitboard of blocker mask (potential blocker sqs - full set of rank and file for given square, EDGE EXCLUSIVE)
Bitboard build_blocker_mask(const Square sq) {
    Bitboard result = square_file_mask(sq) ^ square_rank_mask(sq);
    if (sq_to_file(sq) != FileI) result &= ~file_mask<FileI>();   // crops out the edge squares, unless origin square is on an edge
    if (sq_to_file(sq)!= FileA) result &= ~file_mask<FileA>();
    if (sq_to_rank(sq) != Rank9) result &= ~rank_mask<Rank9>();
    if (sq_to_rank(sq) != Rank1) result &= ~rank_mask<Rank1>();
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
