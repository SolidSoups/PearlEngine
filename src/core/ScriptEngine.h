#pragma once

#include <sol/sol.hpp>
#include "ecs_common.h"

class ScriptComponent;
class Scene;

class ScriptEngine{
public:
  void Init(Scene* scene);
  void RunOnCreate(ecs::Entity entity, ScriptComponent& cp);
  void RunOnUpdate(ecs::Entity entity, ScriptComponent& cp);
  void RunOnDestroy(ecs::Entity entity, ScriptComponent& cp);
  void ReloadScript(ecs::Entity entity, ScriptComponent& cp);

private:
  // binds transform, Vec3, Scene table, Time, Input, Key constants
  void BindAPIs(); 
  // entity injected as "entity"
  sol::environment CreateEntityEnv(ecs::Entity e); 
  sol::state m_Lua;
  Scene* m_Scene = nullptr;
};
