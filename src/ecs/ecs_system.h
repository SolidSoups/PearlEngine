#pragma once

#include <set>

#include "ecs_common.h"

namespace ecs {
class System{
public:
  std::set<Entity> mEntities;
};
};
