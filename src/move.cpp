#include "move.hpp"
#include "position.hpp"
#include "sides.hpp"
#include "bitboard.hpp"

static Move* generate_attacker_moves(const Position& pos, Move* moveList);
static Move* generate_defender_moves(const Position& pos, Move* moveList, Bitboard* allToSquares);
static Move* generate_king_moves(const Position& pos, Move* moveList, Bitboard* allToSquares);


Move* generate(const Position& pos, Move* moveList, Bitboard* allToSquares) {
    Side us = pos.side_to_move();

    return us == Attackers ? generate_attacker_moves(pos, moveList)
                        : generate_defender_moves(pos, moveList, allToSquares);

}

// Make some tests for cross referencing moves vs manual checking
static Move* generate_attacker_moves(const Position& pos, Move* moveList) {
    Bitboard attacker_pieces = pos.attacker_bb();
    Bitboard occupied = pos.occupied_bb();

    while (attacker_pieces) {
        Square from = attacker_pieces.bitscan_forward();

        Bitboard moves = get_moves_unmasked(from, occupied);
        moves &= (~occupied & THRONE_OUT_MASK);  // getting rid of blockers and throne square

        while (moves) {
            Square to = moves.bitscan_pop_forward();
            *moveList++ = encodeMove(Attacker, from, to);
        }
        attacker_pieces.clear_bit(from);
    }
    return moveList;
}

// keeping separate for clarity, plus might want to have some unique events happen during each movegen later (ie, edge detection for surrounds)
static Move* generate_defender_moves(const Position& pos, Move* moveList, Bitboard* allToSquares) {
    Bitboard defender_pieces = pos.defender_bb();
    Bitboard occupied = pos.occupied_bb();
    Bitboard composite_moves = all_zero_bb();

    *allToSquares = all_zero_bb();

    while (defender_pieces) {
        Square from = defender_pieces.bitscan_forward();

        Bitboard moves = get_moves_unmasked(from, occupied);
        moves &= (~occupied & THRONE_OUT_MASK);

        *allToSquares |= moves;  // ORing in all possible destination squares to save some time recognising surround wins

        while (moves) {
            Square to = moves.bitscan_pop_forward();
            *moveList++ = encodeMove(Defender, from, to);
        }
        defender_pieces.clear_bit(from);
    }

    moveList = generate_king_moves(pos, moveList, allToSquares);

    return moveList;
}

static Move* generate_king_moves(const Position& pos, Move* moveList, Bitboard* allToSquares) {
    Square from = pos.king_index();  // based on KING INDEX, not Bitboard. Would cause problems if King is off the board (shouldn't happen)
    Bitboard occupied = pos.occupied_bb();

    Bitboard moves = get_moves_unmasked(from, occupied);
    moves &= ~occupied;

    *allToSquares |= moves;  // the allToSquares bb will already be set to zero in defender moves

    while (moves) {
        Square to = moves.bitscan_pop_forward();
        *moveList++ = encodeMove(King, from, to);
    }
    return moveList;
}

bool is_valid_move(Move move) {
    Square from = move.from();
    Square to = move.to();
    return (SQA1 <= from && from < SquareNum) &&
           (SQA1 <= to && to < SquareNum) &&
           (from != to);
}
