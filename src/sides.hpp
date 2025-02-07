#pragma once

#include "overloadEnumOperators.hpp"

enum Side {
    Defenders,
    Attackers,
    sideNum
};
OVERLOAD_ENUM_OPERATORS(Side);

constexpr Side operator~(Side side) {
    return Side(side ^ Attackers);
}
