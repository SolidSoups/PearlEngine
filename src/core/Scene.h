#pragma once

#include <cstdio>
#include <memory>
#include <strings.h>
#include <vector>
#include "CameraComponent.h"
#include "AmbientLightData.h"
#include "PointLightComponent.h"
#include "ecs_coordinator.h"
#include "ecs_common.h"

class RenderSystem;
class PointLightSystem;
class Camera;

class Scene{
public:
  Scene();
  ~Scene() = default;

  // Object management
  ecs::Entity CreateEntity(const std::string& name = "Entity");
  ecs::Entity CreatePointLight(const std::string& name = "PointLight");
  ecs::Entity CreateCube(const std::string& name = "Cube");
  void DestroyEntity(ecs::Entity entity);
  void Clear();

  // Scene operations
  void Update();
  void Render(Camera& camera);

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

private:
  ecs::Coordinator m_Coordinator;
  std::vector<ecs::Entity> m_Entities;

private:
  ecs::Entity m_ActiveCamera = ecs::NULL_ENTITY;

  std::shared_ptr<RenderSystem> mRenderSystem;
public:
  std::shared_ptr<PointLightSystem> mPointLightSystem;

public:
  void SetActiveCamera(ecs::Entity cameraEntity);
  ecs::Entity GetActiveCamera() const { return m_ActiveCamera; }
};
