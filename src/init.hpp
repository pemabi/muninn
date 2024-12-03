#pragma once

#include "ifdef.hpp"
#include "common.hpp"
#include "bitboard.hpp"

void init_table();

Bitboard build_blocker_mask(const Square sq);
Bitboard build_attacker_mask(const Square sq);
Bitboard carry_rippler_next(Bitboard& subset, Bitboard& mask);
Bitboard find_moves_otf(Bitboard& occupied, Square square);
Bitboard find_attacks_otf(Bitboard& occupied_allies, Square square);
