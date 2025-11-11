#pragma once

#include "ecs_common.h"
class Component{
public:
  virtual ~Component() = default;
  EntityID owner = 0;
};
