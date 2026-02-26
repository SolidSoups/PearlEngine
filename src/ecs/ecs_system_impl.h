#pragma once

#include "ecs_system.h"
#include "ecs_coordinator.h"

namespace ecs {

template <typename Component> Component &System::Get(Entity e) {
  return mCoord->GetComponent<Component>(e);
}
} // namespace ecs
