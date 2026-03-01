#pragma once

#include "ecs_system.h"
#include "ecs_coordinator.h"

namespace ecs {

template <typename Component> Component &System::Get(Entity e) {
  return mCoord->GetComponent<Component>(e);
}

template<typename Component>
bool System::Has(ecs::Entity e){
  return mCoord->HasComponent<Component>(e);
}


template<typename Component>
bool System::TryGet(ecs::Entity e, Component& cmp){
  if(Has<Component>(e)){
    cmp = Get<Component>(e);
    return true;
  }
  return false;
}
} // namespace ecs
