#pragma once

#include <ecs_coordinator.h>
#include <ecs_system.h>
#include "CameraData.h"

struct TransformComponent;

class CameraSystem : public ecs::System {
public:
  enum CameraMode {
    ENGINE, GAME
  };

public:
  void Init(ecs::Coordinator* coord, CameraData* engineCamera = nullptr);
  void SetActiveCamera(ecs::Entity entity);
  void SetEngineCamera(CameraData* engineCamera);
  void SetAspect(float aspect);

  void GetMatrices(CameraMode mode, glm::mat4 &outView, glm::mat4 &outProj);

private:
  ecs::Coordinator* mCoord = nullptr;
  ecs::Entity mActiveCamera = ecs::NULL_ENTITY;
  CameraData* mEngineCamera = nullptr;
  float mAspect = 1.0f;
};
