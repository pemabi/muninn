#pragma once

#include "ifdef.hpp"
#include "common.hpp"
#include "bitboard.hpp"

void initTable();
Bitboard build_blocker_mask(Square sq);

#if defined FIND_MAGIC
u64 findMagic(const Square square);
#endif
