#pragma once

#include <cstdio>
#include <memory>
#include <strings.h>
#include <vector>
#include <deque>

#include "AmbientLightData.h"
#include "RenderSystem.h"
#include "TimeScore.h"
#include "ecs_coordinator.h"
#include "ecs_common.h"
#include "ScriptEngine.h"
#include "ScriptSystem.h"
#include "CameraSystem.h"
#include "PhysicsSystem.h"
#include "json_common.h"
#include "RuntimeState.h"
#include "TerrainSystem.h"

class MeshManager;

class PointLightSystem;
class Camera;
class TextSystem;

class Scene{
public:
  Scene(const std::shared_ptr<IEngineCamera>& engineCam, const std::shared_ptr<InputManager>& inputMan);
  ~Scene();
  void PostInitialization();

  
  // Create an empty entity, with a name component 
  ecs::Entity CreateEntity(const std::string& name = "Entity");
  ecs::Entity CreatePointLight(const std::string& name = "PointLight");
  ecs::Entity CreateCube(const std::string& name = "Cube");
  ecs::Entity CreatePlane(const std::string& name="Plane");
  ecs::Entity CreateSphere(const std::string& name="Sphere");
  ecs::Entity DuplicateEntity(ecs::Entity entity);
  void DestroyEntity(ecs::Entity entity);
  void DestroyEntityDelayed(ecs::Entity entity);
  void ClearAllEntities();

  inline const std::unordered_set<ecs::Entity>& GetRenderEntities() const {
    return mRenderSystem->GetRenderEntities();
  }

  // Scene operations
  void OnRuntimeStart();
  void OnRuntimeStop();
  // reload to the snapshot editor scene
  void OnSceneReload();
  // reload the CURRENT scene
  void ReloadCurrentScene();
  /// Update the scene world. SHOULD ONLY BE RUN IN EDITOR MODE AS CAN DESTROY ENTITIES
  void Update();
  void Render(CameraSystem::CameraMode mode);
  void RenderUI();

  // Query
  inline size_t GetEntityCount() const { return m_Entities.size(); }
  inline const std::vector<ecs::Entity>& GetEntities() const { return m_Entities; }

  // Coordinator access for component operations
  ecs::Coordinator& GetCoordinator() { return m_Coordinator; }
  const ecs::Coordinator& GetCoordinator() const { return m_Coordinator; }

  // Entity name helpers
  std::string GetEntityName(ecs::Entity entity);
  void SetEntityName(ecs::Entity entity, const std::string& name);

  // Get entities with PointLightComponent (returns entity IDs, not component pointers)
  std::vector<ecs::Entity> GetPointLightEntities() const;

  // Scene-level lighting
  AmbientLightData ambientLight;
public:
  void SaveScene(const char* filepath);
  json CreateSceneJSON();
  void LoadScene(const char* filepath);
  void LoadSceneJSON(const json& j);

  ecs::Entity CreateEntityFromJSON(const json& j);
  const json CreateJSONFromEntity(ecs::Entity entity);

  std::shared_ptr<PointLightSystem> mPointLightSystem;
  std::shared_ptr<TerrainSystem> mTerrainSystem;

  inline void SetCameraPreview(ecs::Entity entity){
    mCameraSystem->SetPreviewCamera(entity);
    mCameraSystem->SetCameraMode(CameraSystem::CameraMode::PREVIEW);
  }
  void SetMeshManager(MeshManager* mng) { mMeshManager = mng; }
private:
  ecs::Coordinator m_Coordinator;
  std::vector<ecs::Entity> m_Entities;
  MeshManager* mMeshManager;

private:
  ecs::Entity m_ActiveCamera = ecs::NULL_ENTITY;

  std::shared_ptr<RenderSystem> mRenderSystem;

  std::shared_ptr<ScriptEngine> mScriptEngine;
  std::shared_ptr<ScriptSystem> mScriptSystem;
  std::shared_ptr<CameraSystem> mCameraSystem;
  std::shared_ptr<PhysicsSystem> mPhysicsSystem;
  std::shared_ptr<TextSystem> mTextSystem;

  ecs::Entity mPreviewCameraEntity = ecs::NULL_ENTITY;
  std::string m_PendingSceneLoad;

public:
  // get level time, formatted, for level 1-3
  std::string GetBestTime(int level);
  std::string GetCurrentTime(int level);
  void StartLevelTimer(int level);
  void EndLevelTimers();
  void LoadLevelTimes();
  void SaveLevelTimes();
  void SetActiveCamera(ecs::Entity cameraEntity);
  ecs::Entity GetActiveCamera() const { return m_ActiveCamera; }
  void SetAspectRatio(float aspect);

  void RequestLoadScene(const std::string& path) { m_PendingSceneLoad = path; }
  bool HasPendingLoad() const { return !m_PendingSceneLoad.empty(); }
  std::string ConsumePendingLoad() { auto p = m_PendingSceneLoad; m_PendingSceneLoad.clear(); return p; }

  inline CameraSystem* GetCameraSystem() const {
    return mCameraSystem.get();
  }

  LevelTime mLevelTime;
  LevelTime mDiskLevelTime;
  json mSceneSnapshot;
  json mCurrentSceneSnapshot;
  std::deque<ecs::Entity> mDestroyQueue;
};

