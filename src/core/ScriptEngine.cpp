#include "ScriptEngine.h"

#include <iostream>

#include "ScriptComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "NameComponent.h"
#include "Time.h"


#include <glm/glm.hpp>

void ScriptEngine::Init(Scene *scene) {
  m_Scene = scene;
  m_Lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string,
                       sol::lib::table);
  BindAPIs();
}

void ScriptEngine::LateInit(InputManager* inputManager) {
  mInputManager = inputManager;
}

bool ScriptEngine::RunOnCreate(ecs::Entity entity, ScriptComponent &sc) {
  LOG_INFO << "Creating lua script";
  sol::environment env = CreateEntityEnv(entity);

  // Safely load the script
  if (!SafeCall_StartScript(env, sc)) {
    return false;
  }

  // get OnCreate function, skip if it doesn't exist
  sol::protected_function fn = env["OnCreate"];
  if (!fn.valid()) {
    return true;
  }

  // call function and validate
  auto r = fn();
  if (!r.valid()) {
    sol::error e = r;
    LOG_ERROR << "[Lua] " << sc.scriptPath << "::OnCreate: " << e.what();
    LogError(sc, e);
    return false;
  }

  return true;
}

void ScriptEngine::LogError(ScriptComponent& sc, sol::error& error){
  if(sc.scriptPath.empty()) return;
  sc.hasError = true;
  mScriptToFailureReason[sc.scriptPath] = error.what();
}

bool ScriptEngine::SafeCall_StartScript(sol::environment& env, ScriptComponent& sc){
  // tell sol to load lua file
  auto result =
      m_Lua.safe_script_file(sc.scriptPath, env, sol::script_pass_on_error);

  // validate file
  if (!result.valid()) {
    sol::error err = result;
    LOG_ERROR << "[Lua] " << sc.scriptPath << ": " << err.what();
    LogError(sc, err);
    return false;
  }
  sc.scriptEnv = env;
  sc.loaded = true;

  return true;
}

void ScriptEngine::RunOnUpdate(ecs::Entity entity, ScriptComponent &sc) {
  sol::protected_function fn = sc.scriptEnv["OnUpdate"];
  if (!fn.valid())
    return;

  UpdateAPIs();

  // try running OnUpdate function
  auto r = fn(Time::deltaTime);
  if (!r.valid()) {
    sol::error e = r;
    LOG_ERROR << "[Lua] " << sc.scriptPath << "::OnUpdate: " << e.what();
    LogError(sc, e);
  }
}

void ScriptEngine::UpdateAPIs(){
  // update time global
  sol::table time = m_Lua["Time"];
  time["deltaTime"] = Time::deltaTime;
  time["time"] = Time::time;
  time["sin_time"] = glm::sin(Time::time);
}

void ScriptEngine::RunOnDestroy(ecs::Entity entity, ScriptComponent &sc) {
  if(!sc.scriptEnv) return;

  // try to find OnDestroy function
  sol::protected_function fn = sc.scriptEnv["OnDestroy"];
  if (!fn.valid()) return; 

  // run OnDestroy and validate
  sol::protected_function_result r = fn();
  if (r.valid()) return;

  // log error
  sol::error e = r;
  LOG_ERROR << "[Lua] " << sc.scriptPath << "::OnDestroy: " << e.what();
  LogError(sc, e);
}

void ScriptEngine::ResetScript(ScriptComponent& sc){
  // reset properties, clear errors
  ClearError(sc.scriptPath);
  sc.scriptEnv = sol::table{};
  sc.enabled = false;
  sc.loaded = false;
  sc.needsReload = false;
  sc.hasError = false;
}

void ScriptEngine::ReloadScript(ecs::Entity entity, ScriptComponent &sc) {
  RunOnDestroy(entity, sc);
  sc.enabled = true; // force engine to load script on next frame
}

sol::environment ScriptEngine::CreateEntityEnv(ecs::Entity entity) {
  sol::environment env(m_Lua, sol::create, m_Lua.globals());
  env["entity"] = entity;
  return env;
}

void ScriptEngine::BindAPIs() {
  // bind vec3 type
  m_Lua.new_usertype<glm::vec3>(
      "Vec3", sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
      "x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);

  // transform comp
  m_Lua.new_usertype<TransformComponent>(
      "Transform", "position", &TransformComponent::position, "rotation",
      &TransformComponent::rotation, "scale", &TransformComponent::scale,
      "Translate", &TransformComponent::Translate, "SetPosition",
      &TransformComponent::SetPosition, "SetRotation",
      &TransformComponent::SetRotation, "SetScale",
      &TransformComponent::SetScale);

  // scene table
  sol::table scene = m_Lua.create_named_table("Scene");
  scene.set_function(
      "FindEntityByName", [this](const std::string &name) -> ecs::Entity {
        auto &coord = m_Scene->GetCoordinator();
        for (auto e : m_Scene->GetEntities()) {
          if (coord.HasComponent<NameComponent>(e)) {
            if (coord.GetComponent<NameComponent>(e).name == name)
              return e;
          }
        }
        return ecs::NULL_ENTITY;
      });
  scene.set_function("GetTransform",
                     [this](ecs::Entity e) -> TransformComponent * {
                       auto &coord = m_Scene->GetCoordinator();
                       if (coord.HasComponent<TransformComponent>(e))
                         return &coord.GetComponent<TransformComponent>(e);
                       return nullptr;
                     });

  // time table
  sol::table time = m_Lua.create_named_table("Time");
  time["deltaTime"] = 0.0f;
  time["time"] = 0.0f;
  time["sin_time"] = 0.0f;

  // Logging
  sol::table debug = m_Lua.create_named_table("Debug");
  debug.set_function(
      "Log", [this](const std::string &text) -> void {
      std::cout << "[LuaLog] " << text << std::endl;
    });
  debug.set_function(
      "Warn", [this](const std::string &text) -> void {
      std::cout << "[LuaWarn] " << text << std::endl;
    });
  debug.set_function(
    "Error", [this](const std::string &text) -> void {
      std::cerr << "[LuaError] " << text << std::endl;
    });

  // Input
  sol::table input = m_Lua.create_named_table("Input");
  input.set_function(
    "GetKey", [this](const std::string& key) -> bool{
      auto inputMan = ServiceLocator::Get<InputManager>();
      return inputMan.GetKeyString(key);
    }
  );
  input.set_function(
    "GetKeyDown", [this](const std::string& key) -> bool{
      auto inputMan = ServiceLocator::Get<InputManager>();
      return inputMan.GetKeyDownString(key);
    }
  );
  input.set_function(
    "GetKeyUp", [this](const std::string& key) -> bool{
      auto inputMan = ServiceLocator::Get<InputManager>();
      return inputMan.GetKeyUpString(key);
    }
  );
}
