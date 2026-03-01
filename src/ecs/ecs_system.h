#pragma once

#include <unordered_set>
#include <memory>

#include "ecs_common.h"


namespace ecs {
// fwd declar coordinator
class Coordinator;

// system declaration
class System {
public:
  // set is filled automatically with signed entities
  std::unordered_set<Entity> Entities;

public:
  // interface to set the coordinator on creation
  inline void InitializeSystem(ecs::Coordinator* coord) { mCoord = coord; }

protected:
  ecs::Coordinator* mCoord;
  
  // helper func, really unecessary and quite the luxury
  template<typename Component>
  Component& Get(ecs::Entity e);

  template<typename Component>
  bool Has(ecs::Entity e);

  template<typename Component>
  bool TryGet(ecs::Entity e, Component& cmp);
};
}; // namespace ecs
