#include "CameraSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CameraComponent.h"
#include "TransformComponent.h"

void CameraSystem::Init(ecs::Coordinator *coord, const std::shared_ptr<IEngineCamera> &engineCamera) {
  mCoord = coord;
  mEngineCamera = engineCamera; 
}

void CameraSystem::SetActiveCamera(ecs::Entity entity) {
  mActiveCamera = entity;
}

void CameraSystem::SetAspect(float aspect) { mAspect = aspect; }

void CameraSystem::GetMatrices(CameraMode mode, glm::mat4 &outView,
                               glm::mat4 &outProj) {
  // fallback to engine camera otherwise
  if (mode == ENGINE || mActiveCamera == ecs::NULL_ENTITY) {
    if (mEngineCamera) {
      outView = mEngineCamera->GetViewMatrix();
      outProj = mEngineCamera->GetProjectionMatrix(mAspect);
    } else {
      LOG_ERROR
          << "Engine Camera unavailable, giving 1.0f matrices. EngineCamera("
          << (mEngineCamera) << "), ActiveCamera("
          << (mActiveCamera != ecs::NULL_ENTITY) << ")";
      outView = glm::mat4(1.0f);
      outProj = glm::mat4(1.0f);
    }
    return;
  }

  // view one of the selected cameras
  auto &tf = mCoord->GetComponent<TransformComponent>(mActiveCamera);
  auto &cam = mCoord->GetComponent<CameraComponent>(mActiveCamera);
  outView = glm::lookAt(tf.position, tf.position + tf.GetForward(), tf.GetUp());
  outProj =
      glm::perspective(glm::radians(cam.fov), cam.aspectModifier * mAspect,
                       cam.nearPlane, cam.farPlane);
}

glm::vec3 CameraSystem::GetPosition(CameraMode mode){
  // get engine camera
  if(mode == ENGINE || mActiveCamera == ecs::NULL_ENTITY){
    if(mEngineCamera)
      return mEngineCamera->GetPosition();
    else{
      LOG_ERROR
          << "Engine Camera unavailable, giving 1.0f matrices. EngineCamera("
          << (mEngineCamera) << "), ActiveCamera("
          << (mActiveCamera != ecs::NULL_ENTITY) << ")";
      return glm::vec3(0.0f);
    }
  }

  // get active camera 
  auto activeTransform = mCoord->GetComponent<TransformComponent>(mActiveCamera);
  return activeTransform.position;
}
