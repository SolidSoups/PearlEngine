#pragma once

#include <cstdint>
#include "structs.h"


enum ComponentFlags : uint32_t {
  ComponentFlags_None = 0,

  ComponentFlags_SortHighest = 1 << 0,

  ComponentFlags_MAX
};
ENABLE_BITMASK_OPERATORS(ComponentFlags)
