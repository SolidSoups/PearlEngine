#pragma once

#include "ecs_system.h"

class PhysicsSystem : public ecs::System{
public:
  void UpdatePhysics();
  void DrawGizmos();

private:
};
