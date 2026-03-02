#include "CameraSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CameraComponent.h"
#include "TransformComponent.h"

void CameraSystem::Init(ecs::Coordinator *coord,
                        const std::shared_ptr<IEngineCamera> &engineCamera) {
  mCoord = coord;
  mEngineCamera = engineCamera;
}

void CameraSystem::SetActiveCamera(ecs::Entity entity) {
  // if we have a previous active camera, update isMainCamera to be false
  if (mActiveCamera != ecs::NULL_ENTITY) {
    auto camComp = mCoord->GetComponent<CameraComponent>(mActiveCamera);
    camComp.isMainCamera = false;
  }

  // set new main camera
  mActiveCamera = entity;
  auto camComp = mCoord->GetComponent<CameraComponent>(mActiveCamera);
  camComp.isMainCamera = true;
}

void CameraSystem::SetAspect(float aspect) { mAspect = aspect; }

void CameraSystem::GetMatrices(glm::mat4 &outView, glm::mat4 &outProj) const {
  if (mCameraMode != ENGINE) {
    ecs::Entity cameraEntity = mCameraMode == GAME ? mActiveCamera : mPreviewCamera;
    if (cameraEntity != ecs::NULL_ENTITY &&
        mCoord->HasComponent<TransformComponent>(cameraEntity) &&
        mCoord->HasComponent<CameraComponent>(cameraEntity)) {
      auto &tf = mCoord->GetComponent<TransformComponent>(cameraEntity);
      auto &cam = mCoord->GetComponent<CameraComponent>(cameraEntity);
      outView = glm::lookAt(tf.position, tf.position + tf.GetForward(), tf.GetUp());
      outProj = glm::perspective(glm::radians(cam.fov), cam.aspectModifier * mAspect,
                                 cam.nearPlane, cam.farPlane);
      return;
    }
  }

  // fallback to engine camera
  if (mEngineCamera) {
    outView = mEngineCamera->GetViewMatrix();
    outProj = mEngineCamera->GetProjectionMatrix(mAspect);
  } else {
    outView = glm::mat4(1.0f);
    outProj = glm::mat4(1.0f);
  }
}

glm::vec3 CameraSystem::GetPosition() const {
  if (mCameraMode != ENGINE) {
    ecs::Entity cameraEntity = mCameraMode == GAME ? mActiveCamera : mPreviewCamera;
    if (cameraEntity != ecs::NULL_ENTITY &&
        mCoord->HasComponent<TransformComponent>(cameraEntity))
      return mCoord->GetComponent<TransformComponent>(cameraEntity).position;
  }

  return mEngineCamera ? mEngineCamera->GetPosition() : glm::vec3(0.0f);
}
