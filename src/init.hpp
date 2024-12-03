#pragma once

#include "ifdef.hpp"
#include "common.hpp"
#include "bitboard.hpp"

void init_table();

u64 find_magic(const Square square);
Bitboard build_blocker_mask(const Square sq);
Bitboard carry_rippler_next(Bitboard& subset, Bitboard& mask);
