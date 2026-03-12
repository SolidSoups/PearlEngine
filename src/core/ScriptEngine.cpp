#include "ScriptEngine.h"

#include <iostream>

#include <glm/glm.hpp>

#include "ScriptComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "RigidBodyComponent.h"
#include "NameComponent.h"
#include "Time.h"
#include "LineRenderer.h"
#include "Logger.h"

void ScriptEngine::Init(Scene *scene,
                        const std::shared_ptr<InputManager> &inputMan) {
  mScene = scene;
  m_Lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string,
                       sol::lib::table);
  mInputMan = inputMan;
  BindAPIs();
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

void ScriptEngine::LogError(ScriptComponent &sc, sol::error &error) {
  if (sc.scriptPath.empty())
    return;
  sc.hasError = true;
  mScriptToFailureReason[sc.scriptPath] = error.what();
}

bool ScriptEngine::SafeCall_StartScript(sol::environment &env,
                                        ScriptComponent &sc) {
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
void ScriptEngine::RunOnLateUpdate(ecs::Entity entity, ScriptComponent &sc) {
  sol::protected_function fn = sc.scriptEnv["OnLateUpdate"];
  if (!fn.valid())
    return;

  UpdateAPIs();

  // try running OnUpdate function
  auto r = fn(Time::deltaTime);
  if (!r.valid()) {
    sol::error e = r;
    LOG_ERROR << "[Lua] " << sc.scriptPath << "::OnLateUpdate: " << e.what();
    LogError(sc, e);
  }
}
void ScriptEngine::RunOnCollisionEnter(ecs::Entity entity, ScriptComponent &sc,
                                       ecs::Entity other, glm::vec3 normal,
                                       float penetration) {
  if (!sc.loaded or !sc.enabled or sc.hasError)
    return;
  sol::protected_function fn = sc.scriptEnv["OnCollisionEnter"];
  if (!fn.valid())
    return;

  auto r = fn(other, normal, penetration);
  if (!r.valid()) {
    sol::error e = r;
    LOG_ERROR << "[Lua] " << sc.scriptPath
              << "::OnCollisionEnter: " << e.what();
    LogError(sc, e);
  }
}

void ScriptEngine::RunOnClick(ecs::Entity entity, ScriptComponent& sc) {
  RunScriptFunc(entity,sc, "OnClick");
}

void ScriptEngine::RunScriptFunc(ecs::Entity entity, ScriptComponent& sc, const std::string& funcName){
  if (!sc.loaded || !sc.enabled || sc.hasError) return;
  sol::protected_function fn = sc.scriptEnv[funcName];
  if (!fn.valid()) return;
  auto r = fn();
  if (!r.valid()) {
    sol::error e = r;
    LOG_ERROR << "[Lua] " << sc.scriptPath << "::" << funcName << ": " << e.what();
    LogError(sc, e);
  }
}

void ScriptEngine::UpdateAPIs() {
  // update time global
  sol::table time = m_Lua["Time"];
  time["deltaTime"] = Time::deltaTime;
  time["time"] = Time::time;
  time["sin_time"] = glm::sin(Time::time);
}

void ScriptEngine::RunOnDestroy(ecs::Entity entity, ScriptComponent &sc) {
  if (!sc.scriptEnv)
    return;

  // try to find OnDestroy function
  sol::protected_function fn = sc.scriptEnv["OnDestroy"];
  if (!fn.valid())
    return;

  // run OnDestroy and validate
  sol::protected_function_result r = fn();
  if (r.valid())
    return;

  // log error
  sol::error e = r;
  LOG_ERROR << "[Lua] " << sc.scriptPath << "::OnDestroy: " << e.what();
  LogError(sc, e);
}

void ScriptEngine::ResetScript(ScriptComponent &sc) {
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
      "x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z, "Normalize",
      [](glm::vec3 &v) { v = glm::normalize(v); }, "Length",
      [](const glm::vec3 &v) { return glm::length(v); },

      // arithmetic
      sol::meta_function::addition,
      [](const glm::vec3 &a, const glm::vec3 &b) { return a + b; },
      sol::meta_function::subtraction,
      [](const glm::vec3 &a, const glm::vec3 &b) { return a - b; },
      sol::meta_function::multiplication,
      sol::overload([](const glm::vec3 &v, float s) { return v * s; },
                    [](float s, const glm::vec3 &v) { return v * s; }),
      sol::meta_function::division,
      [](const glm::vec3 &v, float s) { return v / s; });

  // bind collision event

  // transform comp
  m_Lua.new_usertype<NameComponent>(
    "NameComponent", 
    "name", &NameComponent::name
  );
  m_Lua.new_usertype<TransformComponent>(
      "Transform", "position", &TransformComponent::position, "rotation",
      &TransformComponent::rotation, "scale", &TransformComponent::scale,
      "Translate", &TransformComponent::Translate, "SetPosition",
      &TransformComponent::SetPosition, "SetRotation",
      &TransformComponent::SetRotation, "SetScale",
      &TransformComponent::SetScale, "LookAt", &TransformComponent::LookAt);

  // camera comp
  m_Lua.new_usertype<CameraComponent>(
      "Camera", "fov", &CameraComponent::fov, "aspect_mod",
      &CameraComponent::aspectModifier, "near_plane",
      &CameraComponent::nearPlane, "far_plane", &CameraComponent::farPlane,
      "IsMainCamera", &CameraComponent::IsMainCamera);

  // RigidBody
  m_Lua.new_usertype<RigidBodyComponent>(
      "Rigidbody", "mass", &RigidBodyComponent::mass, "gravity",
      &RigidBodyComponent::gravity, "velocity", &RigidBodyComponent::velocity,
      "force", &RigidBodyComponent::force,

      // methods
      "AddForce", &RigidBodyComponent::AddForce, "AddImpulse",
      &RigidBodyComponent::AddImpulse, "ClearForces",
      &RigidBodyComponent::ClearForces);

  // Text Component
  m_Lua.new_usertype<TextComponent>(
    "Text",
    "text", &TextComponent::text,
    "color", &TextComponent::color,
    "isVisible", &TextComponent::isVisible,
    "isButton", &TextComponent::isButton,
    "onClick", &TextComponent::onClick,
    "Remesh", [](TextComponent& self) { self.isDirty = true; }
  );

  // scene table
  auto &coord = mScene->GetCoordinator();
  sol::table scene = m_Lua.create_named_table("Scene");
  scene.set_function(
      "FindEntityByName", [this](const std::string &name) -> ecs::Entity {
        auto &coord = mScene->GetCoordinator();
        for (auto e : mScene->GetEntities()) {
          if (coord.HasComponent<NameComponent>(e)) {
            if (coord.GetComponent<NameComponent>(e).name == name)
              return e;
          }
        }
        return ecs::NULL_ENTITY;
      });
  scene.set_function("GetTransform",
                     [this](ecs::Entity e) -> TransformComponent * {
                       auto &coord = mScene->GetCoordinator();
                       if (coord.HasComponent<TransformComponent>(e))
                         return &coord.GetComponent<TransformComponent>(e);
                       return nullptr;
                     });
  scene.set_function("GetRigidbody",
                     [this](ecs::Entity e) -> RigidBodyComponent * {
                       auto &coord = mScene->GetCoordinator();
                       if (coord.HasComponent<RigidBodyComponent>(e))
                         return &coord.GetComponent<RigidBodyComponent>(e);
                       return nullptr;
                     });
  scene.set_function("GetCamera", [this, &coord](ecs::Entity e) -> CameraComponent * {
    if (coord.HasComponent<CameraComponent>(e))
      return &coord.GetComponent<CameraComponent>(e);
    return nullptr;
  });
  scene.set_function("GetNameComp", [this, &coord](ecs::Entity e) -> NameComponent* {
    if(coord.HasComponent<NameComponent>(e)){
      return &coord.GetComponent<NameComponent>(e);
    }
    return nullptr;
  });
  scene.set_function("GetText", [this, &coord](ecs::Entity e) -> TextComponent* {
    if(coord.HasComponent<TextComponent>(e)){
      return &coord.GetComponent<TextComponent>(e);
    }
    return nullptr;
  });
  scene.set_function("ReloadCurrentScene", [this](){
    mScene->ReloadCurrentScene();
  });
  scene.set_function("DestroyEntity", [this](ecs::Entity entity){
    mScene->DestroyEntityDelayed(entity);
  });

  scene.set_function("SetMainCamera", [this](ecs::Entity e) -> bool {
    LOG_INFO << "Setting main camera";
    if (e == ecs::NULL_ENTITY) {
      LOG_ERROR << "[LUA] SetMainCamera: cannot set main camera of null entity";
      return false;
    }
    LOG_INFO << "Checked for null entity";
    auto &coord = mScene->GetCoordinator();
    if (!coord.HasComponent<TransformComponent>(e)) {
      LOG_ERROR
          << "[LUA] SetMainCamera: entity doesn't have transform component";
      return false;
    }
    LOG_INFO << "Checked for Transform";
    if (!coord.HasComponent<CameraComponent>(e)) {
      LOG_ERROR << "[LUA] SetMainCamera: entity doesn't have camera component";
      return false;
    }
    LOG_INFO << "Checked for Camera";

    // TODO: switch to set active camera
    mScene->SetCameraPreview(e);
    LOG_INFO << "Set camera preview";
    return true;
  });

  scene.set_function("LoadScene", [this](const std::string &path) {
    mScene->RequestLoadScene(path);
  });

  scene["NULL_ENTITY"] = ecs::NULL_ENTITY;

  // time table
  sol::table time = m_Lua.create_named_table("Time");
  time["deltaTime"] = 0.0f;
  time["time"] = 0.0f;
  time["sin_time"] = 0.0f;

  // Logging
  sol::table debug = m_Lua.create_named_table("Debug");
  debug.set_function("Log", [this](const std::string &text) -> void {
    Logger::Get().LogMessage(text, LogSeverity::INFO, "Lua", 0, "Debug.Log");
  });
  debug.set_function("Warn", [this](const std::string &text) -> void {
    Logger::Get().LogMessage(text, LogSeverity::WARNING, "Lua", 0, "Debug.Warn");
  });
  debug.set_function("Error", [this](const std::string &text) -> void {
    Logger::Get().LogMessage(text, LogSeverity::ERROR, "Lua", 0, "Debug.Error");
  });

  // Input
  sol::table input = m_Lua.create_named_table("Input");
  input.set_function("GetKey", [this](const std::string &key) -> bool {
    return mInputMan->GetKeyString(key);
  });
  input.set_function("GetKeyDown", [this](const std::string &key) -> bool {
    return mInputMan->GetKeyDownString(key);
  });
  input.set_function("GetKeyUp", [this](const std::string &key) -> bool {
    return mInputMan->GetKeyUpString(key);
  });

  // Gizmo
  sol::table gizmo = m_Lua.create_named_table("Gizmo");
  gizmo.set_function("DrawLine", [](glm::vec3 a, glm::vec3 b, glm::vec3 color) {
    LineRenderer::DrawLine(a, b, color);
  });
  gizmo.set_function("DrawWireBox",
                     [](glm::vec3 center, glm::vec3 size, glm::vec3 color) {
                       LineRenderer::DrawWireBox(center, size, color);
                     });
  gizmo.set_function("DrawWireSphere",
                     [](glm::vec3 center, float radius, glm::vec3 color) {
                       LineRenderer::DrawWireSphere(center, radius, color);
                     });
}
