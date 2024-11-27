#pragma once

#define OVERLOAD_ENUM_OPERATORS(T)  \
    inline T& operator += (T& lhs, const int rhs) { return lhs = static_cast<T>(static_cast<int>(lhs) + rhs); }  \
    inline T& operator -= (T& lhs, const int rhs) { return lhs = static_cast<T>(static_cast<int>(lhs) - rhs); }  \
    inline T operator ++ (T& lhs) { lhs += 1; return lhs; }   \
    inline T operator -- (T& lhs) { const T temp = lhs; lhs -= 1; return temp; }
