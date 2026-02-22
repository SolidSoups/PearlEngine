#pragma once

#include <sol/sol.hpp>
#include "ecs_common.h"
#include "InputManager.h"
#include "ServiceLocator.h"

class ScriptComponent;
class Scene;

class ScriptEngine{
public:
  void Init(Scene* scene);
  void LateInit(InputManager* inputManager);
  bool RunOnCreate(ecs::Entity entity, ScriptComponent& sc);
  void RunOnUpdate(ecs::Entity entity, ScriptComponent& sc);
  void RunOnDestroy(ecs::Entity entity, ScriptComponent& sc);
  void ResetScript(ScriptComponent& sc);
  void ReloadScript(ecs::Entity entity, ScriptComponent& sc);

  inline bool HasError(const std::string& luaFile) const{
    return mScriptToFailureReason.find(luaFile) != mScriptToFailureReason.end();
  }
  inline const std::string GetError(const std::string& luaFile) const{
    auto it = mScriptToFailureReason.find(luaFile);
    if(it == mScriptToFailureReason.end())
      return "";
    return it->second;
  }
private:
  bool SafeCall_StartScript(sol::environment& env, ScriptComponent& sc);
  void UpdateAPIs();

private:
  // binds transform, Vec3, Scene table, Time, Input, Key constants
  void BindAPIs(); 
  // entity injected as "entity"
  sol::environment CreateEntityEnv(ecs::Entity e); 
  sol::state m_Lua;
  Scene* m_Scene = nullptr;
  InputManager* mInputManager;


private:
  // if we have an entry here, there is a failure
  std::unordered_map<std::string, std::string> mScriptToFailureReason;

  void LogError(ScriptComponent& sc, sol::error& error);
  inline void ClearError(const std::string& luaFile) {
    mScriptToFailureReason.erase(luaFile);
  }
};
