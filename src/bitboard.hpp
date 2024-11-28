
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
