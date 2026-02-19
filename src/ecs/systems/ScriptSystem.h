#pragma once

#include "ecs_system.h"
#include "ecs_coordinator.h"

class ScriptEngine;

class ScriptSystem : public ecs::System {
public:
  void Init(ecs::Coordinator* coordinator, ScriptEngine* engine);
  void OnUpdate();
  void OnDestroy();

private:
  ScriptEngine* mEngine;
  ecs::Coordinator* mCoordinator;
};
