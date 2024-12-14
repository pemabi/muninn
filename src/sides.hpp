#pragma once

#include "overloadEnumOperators.hpp"

enum Side {
    Attack,
    Defense
};
OVERLOAD_ENUM_OPERATORS(Side);
