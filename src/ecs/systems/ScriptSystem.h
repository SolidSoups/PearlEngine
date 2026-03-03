#pragma once

#include <unordered_map>
#include <unordered_set>

#include "ecs_system.h"
#include "ecs_coordinator.h"

class ScriptEngine;

class ScriptSystem : public ecs::System {
public:
  void Init(ecs::Coordinator* coordinator, ScriptEngine* engine);
  void OnCreateScene();
  void OnUpdate();
  void OnDestroy();
  void DispatchOnCollisionEnter(ecs::Entity a, ecs::Entity b, const glm::vec3& normal, float penetration);

private:
  void TryDispatch(ecs::Entity a, ecs::Entity b, const glm::vec3& normal, float pen);
  ScriptEngine* mEngine;
  ecs::Coordinator* mCoordinator;
};
