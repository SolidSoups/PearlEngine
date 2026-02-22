#include "CameraSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CameraComponent.h"
#include "TransformComponent.h"

void CameraSystem::Init(ecs::Coordinator* coord, CameraData* engineCamera) {
  mCoord = coord;
  mEngineCamera = engineCamera;
}

void CameraSystem::SetActiveCamera(ecs::Entity entity) {
  mActiveCamera = entity;
}

void CameraSystem::SetEngineCamera(CameraData* engineCamera) {
  mEngineCamera = engineCamera;
}

void CameraSystem::SetAspect(float aspect) {
  mAspect = aspect;
}

void CameraSystem::GetMatrices(CameraMode mode, glm::mat4 &outView, glm::mat4 &outProj) {
  // Fallback to ENGINE if GAME mode has no active camera
  if (mode == ENGINE || mActiveCamera == ecs::NULL_ENTITY) {
    if (mEngineCamera) {
      outView = mEngineCamera->GetViewMatrix();
      outProj = mEngineCamera->GetProjectionMatrix();
    } else {
      outView = glm::mat4(1.0f);
      outProj = glm::mat4(1.0f);
    }
    return;
  }

  // GAME mode: derive matrices from the active camera entity's components
  auto& tf  = mCoord->GetComponent<TransformComponent>(mActiveCamera);
  auto& cam = mCoord->GetComponent<CameraComponent>(mActiveCamera);
  outView = glm::lookAt(tf.position, tf.position + tf.GetForward(), tf.GetUp());
  outProj = glm::perspective(glm::radians(cam.fov), cam.aspectModifier * mAspect, cam.nearPlane, cam.farPlane);
}
