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
  // fallback to engine camera otherwise
  if (mCameraMode == ENGINE || mActiveCamera == ecs::NULL_ENTITY ||
      mPreviewCamera == ecs::NULL_ENTITY) {
    if (mEngineCamera) {
      outView = mEngineCamera->GetViewMatrix();
      outProj = mEngineCamera->GetProjectionMatrix(mAspect);
    } else {
      LOG_ERROR
          << "Engine Camera unavailable, giving 1.0f matrices. \nEngineCamera("
          << (mEngineCamera) << "), \n"
          << "ActiveCamera(" << (mActiveCamera != ecs::NULL_ENTITY) << "), \n"
          << "PreviewCamera(" << (mPreviewCamera != ecs::NULL_ENTITY) << ")";
      outView = glm::mat4(1.0f);
      outProj = glm::mat4(1.0f);
    }
    return;
  }

  // view one of the active/selected cameras
  ecs::Entity cameraEntity =
      mCameraMode == GAME ? mActiveCamera : mPreviewCamera;
  if (!mCoord->HasComponent<TransformComponent>(cameraEntity) ||
      !mCoord->HasComponent<CameraComponent>(cameraEntity)) {
    LOG_ERROR << "Entity (" << cameraEntity
              << ") has no transform OR camera component";
    outView = {1.0};
    outProj = {1.0};
    return;
  }

  // get matrices
  auto &tf = mCoord->GetComponent<TransformComponent>(cameraEntity);
  auto &cam = mCoord->GetComponent<CameraComponent>(cameraEntity);
  outView = 
    glm::lookAt(tf.position, tf.position + tf.GetForward(), tf.GetUp());
  outProj =
      glm::perspective(glm::radians(cam.fov), cam.aspectModifier * mAspect,
                       cam.nearPlane, cam.farPlane);
}

glm::vec3 CameraSystem::GetPosition() const {
  // get engine camera
  if (mCameraMode == ENGINE || mActiveCamera == ecs::NULL_ENTITY || mPreviewCamera == ecs::NULL_ENTITY) {
    if (mEngineCamera)
      return mEngineCamera->GetPosition();
    else {
      LOG_ERROR
          << "Engine Camera unavailable, giving 1.0f matrices. EngineCamera("
          << (mEngineCamera) << "), ActiveCamera("
          << (mActiveCamera != ecs::NULL_ENTITY) << ")";
      return glm::vec3(0.0f);
    }
  }

  // get active camera
  auto activeTransform = mCameraMode == GAME ?
      mCoord->GetComponent<TransformComponent>(mActiveCamera) 
      : mCoord->GetComponent<TransformComponent>(mPreviewCamera);
  return activeTransform.position;
}
