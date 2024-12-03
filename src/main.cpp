#include <iostream>
#include "bitboard.hpp"
#include "init.hpp"
#include "common.hpp"

int main() {
    RandomGenerator rng;

    for (int i=0; i<5; ++i) {
        u64 low = rng.random_u64_sparse();
        u64 high = rng.random_u64_sparse();

        low = low & UINT64_C(0x7FFFFFFFFFFFFFFF);
        high = high & UINT64_C(0x000000000003FFFF);

        Bitboard bb = Bitboard(low, high);

        bb.print_board();
    }
}

int find_magic()  {
    u64 MAGIC[SquareNum];

    std::cout << "const u64 MAGIC[81] = {" <<std::endl;
    for (Square sq = SQA1; sq < SquareNum; ++sq)  {
        MAGIC[sq] = find_magic(sq);
        std::cout << "\tUINT64_C(0x" << std::hex << MAGIC[sq] << ")," << std::endl;
    }
    std::cout << "};\n" << std::endl;

    return 0;
}
