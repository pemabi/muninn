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

// NEED TO FIX BLOCKERS?
// Make some tests for cross referencing moves vs manual checking
static Move* generate_attacker_moves(const Position& pos, Move* moveList) {
    Bitboard attacker_pieces = pos.attackers();
    Bitboard occupied = pos.occupied();
    Bitboard occupied_edges = occupied & EDGE_MASK;

    while (attacker_pieces) {
        Square from = attacker_pieces.bitscan_forward();

        Bitboard moves = get_moves_unmasked(from, occupied);
        moves &= ~occupied;  // getting rid of edge blockers

        while (moves) {
            Square to = moves.bitscan_pop_forward();
            *moveList++ = encodeMove(Attacker, from, to);
        }
        attacker_pieces.clear_bit(from);
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
