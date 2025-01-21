#include "move.hpp"
#include "position.hpp"
#include "sides.hpp"
#include "bitboard.hpp"

static Move* generate_attacker_moves(const Position& pos, Move* moveList);
static Move* generate_defender_moves(const Position& pos, Move* moveList);
static Move* generate_king_moves(const Position& pos, Move* moveList);


Move* generate(const Position& pos, Move* moveList) {
    Side us = pos.side_to_move();

    return us == Attackers ? generate_attacker_moves(pos, moveList)
                        : generate_defender_moves(pos, moveList);

}

static Move* generate_attacker_moves(const Position& pos, Move* moveList) {
    Bitboard attacker_pieces = pos.attackers();
    Bitboard occupied = pos.occupied();
    Bitboard occupied_edges = occupied & EDGE_MASK;

    while (attacker_pieces) {
        Square from = attacker_pieces.bitscan_pop_forward(); // does it matter that I am popping the origin square? will this mess with the magics?

        Bitboard moves = get_moves_unmasked(from, occupied);
        moves &= ~occupied_edges;  // getting rid of edge blockers

        while (moves) {
            Square to = moves.bitscan_pop_forward();  // built in to pop the bit while getting the index
            *moveList++ = encodeMove(Attacker, from, to); // Need to fix piece, piecetypes.
        }
    }
    return moveList;
}

static Move* generate_defender_moves(const Position& pos, Move* moveList) {
    Bitboard defender_pieces = pos.defenders();
    Bitboard occupied = pos.occupied();
    Bitboard occupied_edges = occupied & EDGE_MASK;

    while (defender_pieces) {
        Square from = defender_pieces.bitscan_pop_forward();

        Bitboard moves = get_moves_unmasked(from, occupied);
        moves &= ~occupied_edges;

        while (moves) {
            Square to = moves.bitscan_pop_forward();
            *moveList++ = encodeMove(Defender, from, to);
        }
    }

    moveList = generate_king_moves(pos, moveList);

    return moveList;
}

static Move* generate_king_moves(const Position& pos, Move* moveList) {
    Square from = pos.king_index();
    Bitboard occupied = pos.occupied();

    Bitboard moves = get_moves_unmasked(from, occupied);

    while (moves) {
        Square to = moves.bitscan_pop_forward();
        *moveList++ = encodeMove(King, from, to);
    }
    return moveList;
}
