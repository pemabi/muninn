
# pragma once
#include <iostream>

#include "common.hpp"
#include "sides.hpp"
#include "square.hpp"

class Bitboard;
extern const Bitboard SquareMaskBB[SquareNum];

//todo add support for SSE2, SSE4, BMI2
class Bitboard  {
public:
    Bitboard() {}

    Bitboard(const u64 v0, const u64 v1)  {
        this->p_[0] = v0;  // upper board
        this->p_[1] = v1;  // lower board
    }

    // returns board segment (at 0 or 1)
    u64 p(const int index) const { return p_[index]; }

    // sets board segment (at 0 or 1)
    void set(const int index, const u64 val) { p_[index] = val; }

    // merges board segemnts - useful for magic key generation
    u64 merge() const { return this->p(0) | this->p(1); }

    // converts bb to bool - returns true if any bit set
    explicit operator bool() const {
        return (this->merge() ? true : false);
    }

    // checks if bitboard is empty
    bool is_not_empty() const { return static_cast<bool>(*this); }

    // checks for overlap between this bitboard and another
    bool is_overlap(const Bitboard& bb) const {
        return (*this & bb).is_not_empty();
    }

    // operator overloading, adapt bitwise operators for segmented board structure

    // NOT operator (flips bits)
    Bitboard operator ~ () const {
        return Bitboard(~this->p(0), ~this->p(1));
    }

    // AND operator
    Bitboard operator &= (const Bitboard& bb) {
        this->p_[0] &= bb.p(0);
        this->p_[1] &= bb.p(1);
        return *this;
    }

    // OR operator
    Bitboard operator |= (const Bitboard& bb) {
        this->p_[0] |= bb.p(0);
        this->p_[1] |= bb.p(1);
        return *this;
    }

    // XOR operator
    Bitboard operator ^= (const Bitboard& bb) {
        this->p_[0] ^= bb.p(0);
        this->p_[1] ^= bb.p(1);
        return *this;
    }

    // LEFT SHIFT operator
    Bitboard operator <<= (const int i) {
        this->p_[0] <<= i;
        this->p_[1] <<= i;
        return *this;
    }

    // RIGHT SHIFT operator
    Bitboard operator >>= (const int i) {
        this->p_[0] >>= i;
        this->p_[1] >>= i;
        return *this;
    }

    Bitboard operator & (const Bitboard& bb) const { return Bitboard(*this) &= bb; }
    Bitboard operator | (const Bitboard& bb) const { return Bitboard(*this) |= bb; }
    Bitboard operator ^ (const Bitboard& bb) const { return Bitboard(*this) ^= bb; }
    Bitboard operator << (const int i) const { return Bitboard(*this) <<= i; }
    Bitboard operator >> (const int i) const { return Bitboard(*this) >>= i; }
    bool operator == (const Bitboard& bb) const { return (this->p(0) == bb.p(0)) && (this->p(1) == bb.p(1)); }
    bool operator != (const Bitboard& bb) const { return !(*this == bb); }
    Bitboard and_equal_not(const Bitboard& bb) { return (*this) &= ~bb; } // dont think I need to this as I am not running SSE2/SSE4 optimisations yet. Might as well future proof
    Bitboard not_this_and(const Bitboard& bb) { return ~(*this) & bb; } // ditto for this

    // check if square has a bit set
    bool is_set(const Square sq) const {
        assert(is_in_square(sq));
        return is_overlap(SquareMaskBB[sq]);  // check against mask for square
    }

    // set bit for a square by index
    void set_bit(const Square sq) { *this |= SquareMaskBB[sq]; }

    // Clear bit for a square by index
    void clear_bit(const Square sq) { *this &= ~SquareMaskBB[sq]; }  // AND op with INVERSE of mask

    // toggle bit for a square by index
    void xor_bit(const Square sq) { *this ^= SquareMaskBB[sq]; }  // XOR bb with mask - if empty then returns full and vice versa

    // count total num of bits set on bitboard - more efficient to check for crossover and then merge first if possible?
    int pop_count() const {
        return count_ones(p(0)) + count_ones(p(1));
    }

    // Bitscan board, returning 'smallest' square. forward from lsb, backward from msb
    inline Square bitscan_forward() const {
        if (this->p(0)) {
            return static_cast<Square>(lsb(this->p(0)));
        }
        return static_cast<Square>(lsb(this->p(1)) + 63);
        }

    // Bitscan board and then clear lsb, returns 'smallest' square
    inline Square bitscan_pop_forward() {
        if (this->p(0)) {
            const Square sq = static_cast<Square>(lsb(this->p(0)));
            this->p_[0] &= this->p(0) - 1;
            return sq;
        }
        const  Square sq = static_cast<Square>(lsb(this->p(1)));
        this->p_[1] &= this->p(1) - 1;
        return sq;
    }

    // returns 1 if square is in upper board, 0 if it is in lower board.
    static int part(const Square sq) { return static_cast<int>(sq > CutoffSquareLower); }   //todo: rename!!

    // print set bits on bitboard for debugging
    void print_board() const {
        // For each rank (row)
        for (Rank r = Rank9; r >= Rank1; --r) {
            std::cout << (r+1);  // Print row number
            // For each file (column)
            for (File f = FileA; f < FileNum; ++f)
                // Print X for set bits, . for unset
                std::cout << (this->is_set(index_square(f, r)) ? "  X" : "  .");
            std::cout << "\n";
        }
        std::cout << "   A  B  C  D  E  F  G  H  I\n\n";
        std::cout << "Bitboard\nLOW: "<<this->p_[0]<<"\nHIGH: "<<this->p_[1]<<'\n';
        std::cout << std::endl;
    }

private:
    // array of 2x 64-bit unsigned integers. p_[0] stores lower board (63 bits of 64), p_[0] stores upper board (18 bits of 64)
    u64 p_[2];

};

inline Bitboard get_square_mask(const Square sq)  { return SquareMaskBB[sq]; }

inline Bitboard all_one_bb() { return Bitboard(UINT64_C(0x7FFFFFFFFFFFFFFF), UINT64_C(0x000000000003FFFF)); }
inline Bitboard all_zero_bb() { return Bitboard(0, 0); }
inline Bitboard start_pos() { return Bitboard(UINT64_C(0x04223FF888402038), UINT64_C(0x0000000000007010)); }

extern const int BLOCKER_BITS[SquareNum];
extern const int SHIFT_BITS[SquareNum];

extern const Bitboard FileMask[FileNum];
extern const Bitboard RankMask[RankNum];

const Bitboard File1Mask = Bitboard(UINT64_C(0x40201008040201) << 0, UINT64_C(0x201) << 0);
const Bitboard File2Mask = Bitboard(UINT64_C(0x40201008040201) << 1, UINT64_C(0x201) << 1);
const Bitboard File3Mask = Bitboard(UINT64_C(0x40201008040201) << 2, UINT64_C(0x201) << 2);
const Bitboard File4Mask = Bitboard(UINT64_C(0x40201008040201) << 3, UINT64_C(0x201) << 3);
const Bitboard File5Mask = Bitboard(UINT64_C(0x40201008040201) << 4, UINT64_C(0x201) << 4);
const Bitboard File6Mask = Bitboard(UINT64_C(0x40201008040201) << 5, UINT64_C(0x201) << 5);
const Bitboard File7Mask = Bitboard(UINT64_C(0x40201008040201) << 6, UINT64_C(0x201) << 6);
const Bitboard File8Mask = Bitboard(UINT64_C(0x40201008040201) << 7, UINT64_C(0x201) << 7);
const Bitboard File9Mask = Bitboard(UINT64_C(0x40201008040201) << 8, UINT64_C(0x201) << 8);
const Bitboard File10Mask = Bitboard(UINT64_C(0x40201008040201) << 9, UINT64_C(0x201) << 9);

const Bitboard Rank1Mask = Bitboard(UINT64_C(0x1ff) << (9 * 0), 0);
const Bitboard Rank2Mask = Bitboard(UINT64_C(0x1ff) << (9 * 1), 0);
const Bitboard Rank3Mask = Bitboard(UINT64_C(0x1ff) << (9 * 2), 0);
const Bitboard Rank4Mask = Bitboard(UINT64_C(0x1ff) << (9 * 3), 0);
const Bitboard Rank5Mask = Bitboard(UINT64_C(0x1ff) << (9 * 4), 0);
const Bitboard Rank6Mask = Bitboard(UINT64_C(0x1ff) << (9 * 5), 0);
const Bitboard Rank7Mask = Bitboard(UINT64_C(0x1ff) << (9 * 6), 0);
const Bitboard Rank8Mask = Bitboard(0, UINT64_C(0x1ff) << (9 * 0));
const Bitboard Rank9Mask = Bitboard(0, UINT64_C(0x1ff) << (9 * 1));

extern const u64 MAGIC[SquareNum];
extern  Bitboard BLOCKER_MASK[SquareNum];
extern  Bitboard MOVE[512000];
extern int INDEX_OFFSET[SquareNum]; // In my first implementation, I used a 2D table where the first dimension was square number. I am adopting Apery's 1D approach, where an offset is added to the hashed index by square.

inline Bitboard file_mask(const File f) { return FileMask[f]; }

template <File F> inline Bitboard file_mask() {
    static_assert(FileBegin <= F && F < FileNum, "");
    return (F == FileA ? File1Mask
    : F == FileB ? File2Mask
    : F == FileC ? File3Mask
    : F == FileD ? File4Mask
    : F == FileE ? File5Mask
    : F == FileF ? File6Mask
    : F == FileG ? File7Mask
    : F == FileH ? File8Mask
    : File9Mask);
}

inline Bitboard square_file_mask(const Square sq) {
    const File f = sq_to_file(sq);
    return file_mask(f);
}

inline Bitboard rank_mask(const Rank r) { return RankMask[r]; }

template <Rank R> inline Bitboard rank_mask() {
    static_assert(RankBegin <= R && R < RankNum, "");
    return (R == Rank1 ? Rank1Mask
    : R == Rank2 ? Rank2Mask
    : R == Rank3 ? Rank3Mask
    : R == Rank4 ? Rank4Mask
    : R == Rank5 ? Rank5Mask
    : R == Rank6 ? Rank6Mask
    : R == Rank7 ? Rank7Mask
    : R == Rank8 ? Rank8Mask
    : Rank9Mask);
}

inline Bitboard square_rank_mask(const Square sq) {
    const Rank r = sq_to_rank(sq);
    return rank_mask(r);
}

// Custom hash function for magic representation
inline u64 get_hash_value(const u64 blocker_key, const u64 magic, const int shiftBits) {
    return (blocker_key * magic) >> shiftBits;
}

inline Bitboard get_moves_unmasked(const Square sq, const Bitboard& occupied) {
    const u64 blocker_key = (occupied & BLOCKER_MASK[sq]).merge();
    return MOVE[INDEX_OFFSET[sq] + get_hash_value(blocker_key, MAGIC[sq], SHIFT_BITS[sq])];
}
