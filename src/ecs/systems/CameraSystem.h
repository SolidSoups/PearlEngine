#pragma once

#include <ecs_coordinator.h>
#include <ecs_system.h>

struct TransformComponent;

class CameraSystem : public ecs::System {
public:
  enum CameraMode {
    ENGINE, GAME
  };
  
public:
  void Init(ecs::Coordinator* coordinator);
  void SetActiveCamera(ecs::Entity entity);

  void GetMatrices(CameraMode mode, glm::mat4 &outView, glm::mat4 &outProj);
private:
  glm::mat4 GetViewMatrix();
  ecs::Coordinator* mCoord;
  ecs::Entity mActiveCamera = ecs::NULL_ENTITY;
};
