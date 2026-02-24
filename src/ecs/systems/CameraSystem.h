#pragma once

#include <ecs_coordinator.h>
#include <ecs_system.h>
#include "CameraData.h"
#include "EngineCamera.h"

struct TransformComponent;

class CameraSystem : public ecs::System {
public:
  enum CameraMode {
    ENGINE, GAME
  };

public:
  void Init(ecs::Coordinator* coord, const std::shared_ptr<IEngineCamera>& engineCamera);
  void SetActiveCamera(ecs::Entity entity);
  void SetAspect(float aspect);

  void GetMatrices(CameraMode mode, glm::mat4 &outView, glm::mat4 &outProj);
  glm::vec3 GetPosition(CameraMode mode);

private:
  ecs::Coordinator* mCoord = nullptr;
  ecs::Entity mActiveCamera = ecs::NULL_ENTITY;
  std::shared_ptr<IEngineCamera> mEngineCamera = nullptr;
  float mAspect = 1.0f;

};

