#pragma once

#include <cinttypes>
#include <cinttypes>
#include <array>
#include <tuple>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cassert>
#include <ctime>
#include <cmath>
#include <cstddef>
#include <cfloat>
#include <random>

using s8  =  int8_t;
using u8  = uint8_t;
using s16 =  int16_t;
using u16 = uint16_t;
using s32 =  int32_t;
using u32 = uint32_t;
using s64 =  int64_t;
using u64 = uint64_t;

// Key used for Zobrist
using ZobristKey = u64;

/*
Logic for finding least significant bit / most significant bit. Returns index of LSB.
todo: add logic for different compilers / architectures
https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
*/

// Kim Walisch's De Bruijn multuplication extension
//https://www.chessprogramming.org/BitScan#DeBruijnMultiplation
const int DE_BRUIJN_INDEX[64] = {
    0, 47,  1, 56, 48, 27,  2, 60,
    57, 49, 41, 37, 28, 16,  3, 61,
    54, 58, 35, 52, 50, 42, 21, 44,
    38, 32, 29, 23, 17, 11,  4, 62,
    46, 55, 26, 59, 40, 36, 15, 53,
    34, 51, 20, 43, 31, 22, 10, 45,
    25, 39, 14, 33, 19, 30,  9, 24,
    13, 18,  8, 12,  7,  6,  5, 63
};

const u64 DE_BRUIJN_64 = 0x03f79d71b4cb0a89;

// xor by ones complement
inline int lsb(u64 x) {
    return DE_BRUIJN_INDEX[((x ^ (x-1)) * DE_BRUIJN_64) >> 58];
};

inline int from_msb(u64 x) {
    for (int i = 63; 0 <= i; --i) {
        if (x >> i)
            return 63 - i;
    }
    return 0;
}

inline int msb(u64 x) {
    return 63 - from_msb(x);
}

// counts number of set bits in word
inline int count_ones(u64 x)
{
    x = x - ((x >> 1) & UINT64_C(0x5555555555555555));
    x = (x & UINT64_C(0x3333333333333333)) + ((x >> 2) & UINT64_C(0x3333333333333333));
    x = (x + (x >> 4)) & UINT64_C(0x0f0f0f0f0f0f0f0f);
    x = x + (x >> 8);
    x = x + (x >> 16);
    x = x + (x >> 32);
    return (static_cast<int>(x)) & 0x0000007f;
}

// popcount alogrithm to parallelise counting 1s
inline int count1s(u64 x)
{
    x = x - ((x >> 1) & UINT64_C(0x5555555555555555));
    x = (x & UINT64_C(0x3333333333333333)) + ((x >> 2) & UINT64_C(0x3333333333333333));
    x = (x + (x >> 4)) & UINT64_C(0x0f0f0f0f0f0f0f0f);
    x = x + (x >> 8);
    x = x + (x >> 16);
    x = x + (x >> 32);
    return (static_cast<int>(x)) & 0x0000007f;
}

template<typename T>
void atomic_add(std::atomic<T> &target, T value) {
    T expected = target.load();
    while (!target.compare_exchange_weak(expected, expected + value));
}

// Changed this to include a singleton pattern. Kept the initial class constructor / methods to avoid refactoring.
// TODO: refactor to use singleton pattern
class RandomGenerator {
private:
    std::mt19937_64 mt;

public:
    RandomGenerator() : mt(std::random_device{}()) {}

    // Instance methods with different names
    u64 generate_u64() {
        return mt();
    }

    u64 generate_u64_sparse() {
        return mt() & mt() & mt();
    }

    int generate_int(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(mt);
    }

    std::mt19937_64& get_generator() {
        return mt;
    }

    void seed(uint64_t seed) {
        mt.seed(seed);
    }

    // Static methods keep the clean names
    static RandomGenerator& instance() {
        thread_local static RandomGenerator rng;
        return rng;
    }

    static u64 random_u64() {
        return instance().generate_u64();
    }

    static u64 random_u64_sparse() {
        return instance().generate_u64_sparse();
    }

    static int random_int(int min, int max) {
        return instance().generate_int(min, max);
    }

    static std::mt19937_64& get_rng() {
        return instance().get_generator();
    }

    static void seed_global(uint64_t seed) {
        instance().seed(seed);
    }
};

const int MAX_MOVES = 256;
