#pragma once

#include <unordered_set>

#include "ecs_common.h"

namespace ecs {

class System{
public:
  std::unordered_set<Entity> mEntities;
};
};
