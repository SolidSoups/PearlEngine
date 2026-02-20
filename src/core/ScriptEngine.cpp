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

bool ScriptEngine::RunOnCreate(ecs::Entity entity, ScriptComponent &sc) {
  sol::environment env = CreateEntityEnv(entity);

  auto result =
      m_Lua.safe_script_file(sc.scriptPath, env, sol::script_pass_on_error);
  if (!result.valid()) {
    sol::error err = result;
    LOG_ERROR << "[Lua] " << sc.scriptPath << ": " << err.what();
    sc.failed = true;
    sc.failure_reason = err.what();
    return false;
  }
  sc.scriptEnv = env;

  sol::protected_function fn = env["OnCreate"];
  if (fn.valid()) {
    auto r = fn();
    if (!r.valid()) {
      sol::error e = r;
      LOG_ERROR << "[Lua] " << sc.scriptPath << "::OnCreate: " << e.what();
      sc.failed = true;
      sc.failure_reason += "\n";
      sc.failure_reason += e.what();
      return false;
    }
  }
  return true;
}

void ScriptEngine::RunOnUpdate(ecs::Entity entity, ScriptComponent &sc) {
  sol::protected_function fn = sc.scriptEnv["OnUpdate"];
  if (!fn.valid())
    return;

  sol::table time = m_Lua["Time"];
  time["deltaTime"] = Time::deltaTime;
  time["time"] = Time::time;
  time["sin_time"] = glm::sin(Time::time);

  auto r = fn(Time::deltaTime);
  if (!r.valid()) {
    sol::error e = r;
    LOG_ERROR << "[Lua] " << sc.scriptPath << "::OnUpdate: " << e.what();
    sc.failed = true;
    sc.failure_reason += "\n";
    sc.failure_reason += e.what();
  }
}

void ScriptEngine::RunOnDestroy(ecs::Entity entity, ScriptComponent &sc) {
  sol::protected_function fn = sc.scriptEnv["OnDestroy"];
  if (!fn.valid())
    return;

  auto r = fn();
  if (!r.valid()) {
    sol::error e = r;
    LOG_ERROR << "[Lua] " << sc.scriptPath << "::OnDestroy: " << e.what();
  }
  sc.scriptEnv = sol::environment{};
  sc.loaded = false;
  sc.failed = false;
  sc.needsReload = false;
  sc.failure_reason = "";
  LOG_INFO << "Ran OnDestroy() on lua environment";
}

void ScriptEngine::ReloadScript(ecs::Entity entity, ScriptComponent &sc) {
  RunOnDestroy(entity, sc);
  sc.scriptEnv = sol::table{};
  sc.loaded = false;
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

}
