#include <iostream>
#include "bitboard.hpp"
#include "init.hpp"

int main() {
    u64 MAGIC[SquareNum];

    std::cout << "const u64 MAGIC[81] = {" <<std::endl;
    for (Square sq = SQA1; sq < SquareNum; ++sq)  {
        MAGIC[sq] = find_magic(sq);
        std::cout << "\tUINT64_C(0x" << std::hex << MAGIC[sq] << ")," << std::endl;
    }
    std::cout << "};\n" << std::endl;

    return 0;
}
