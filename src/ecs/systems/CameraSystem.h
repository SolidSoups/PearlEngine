#pragma once

#include <ecs_coordinator.h>
#include <ecs_system.h>
#include "CameraData.h"
#include "EngineCamera.h"
#include "CameraComponent.h"

struct TransformComponent;

class CameraSystem : public ecs::System {
public:
  enum CameraMode {
    ENGINE, PREVIEW, GAME
  };

public:
  void Init(ecs::Coordinator* coord, const std::shared_ptr<IEngineCamera>& engineCamera);
  void SetActiveCamera(ecs::Entity entity);
  inline void SetPreviewCamera(ecs::Entity entity){
    if (!mCoord->HasComponent<CameraComponent>(entity) ||
      !mCoord->HasComponent<TransformComponent>(entity)){
      LOG_ERROR << "Entity does not have enough components";
      mPreviewCamera = ecs::NULL_ENTITY;
      return;
    }
    mPreviewCamera = entity;
  }
  void SetAspect(float aspect);

  void GetMatrices(glm::mat4 &outView, glm::mat4 &outProj) const;
  glm::vec3 GetPosition() const;

  inline void SetCameraMode(CameraMode mode){
    mCameraMode = mode;
    if(mCameraMode == PREVIEW && mPreviewCamera == ecs::NULL_ENTITY)
      LOG_ERROR << "Cannot enter preview mode for null preview camera entity";
  }
  inline CameraMode GetCameraMode() const { return mCameraMode; }

  inline ecs::Entity GetPreviewEntity() const {
    return mPreviewCamera;
  }
private:
  ecs::Coordinator* mCoord = nullptr;
  ecs::Entity mActiveCamera = ecs::NULL_ENTITY;
  std::shared_ptr<IEngineCamera> mEngineCamera = nullptr;
  float mAspect = 1.0f;
  CameraMode mCameraMode = CameraMode::ENGINE;

  ecs::Entity mPreviewCamera = ecs::NULL_ENTITY;
};

