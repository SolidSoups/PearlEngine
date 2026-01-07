#include "Camera.h"
#include "ServiceLocator.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/quaternion.hpp"

Camera::Camera() : r_Scene() {}

void Camera::StartPreview(CameraComponent *target) {
  m_IsPreviewingSceneCamera = true;
  m_PreviewTarget = target;
}

void Camera::SetAspect(float aspect) {
  m_Aspect = aspect;
  m_InternalCameraData.aspectRatio = aspect;
  if (m_PreviewTarget)
    m_PreviewTarget->cameraData.aspectRatio = aspect;
}

CameraData* Camera::GetPreview(){
  if(m_IsPreviewingSceneCamera && m_PreviewTarget){
    // get the gameobject that owns this camera
    GameObject* owner = m_PreviewTarget->GetOwner();
    if(TransformComponent *transform = owner->GetComponent<TransformComponent>()){
      // update position
      m_PreviewTarget->cameraData.position = transform->position;

      // build rotation matrix from euler angles
      glm::quat orientation = glm::quat(glm::vec3(
          glm::radians(transform->rotation.x),
          glm::radians(transform->rotation.y),
          glm::radians(transform->rotation.z)
      ));

      // extract forward and up from matrix
      glm::vec3 forward = orientation * glm::vec3(0, 0, -1); // Local -Z
      glm::vec3 up = orientation * glm::vec3(0, 1, 0); // Local y

      // update camera vectors to match
      m_PreviewTarget->cameraData.target = transform->position + forward;
      m_PreviewTarget->cameraData.worldUp = up;
      m_PreviewTarget->cameraData.UpdateCameraVectors();
    }
    m_PreviewTarget->cameraData.aspectRatio = m_Aspect;
    return &m_PreviewTarget->cameraData;
  }
  return nullptr;
}
CameraData* Camera::GetCurrentTarget(){
  if(auto* preview = GetPreview())
    return preview;
  return &m_InternalCameraData;
}

void Camera::StopPreview() {
  // update internal camera data to match preview
  if(m_IsPreviewingSceneCamera && m_PreviewTarget)
    m_InternalCameraData = m_PreviewTarget->cameraData;

  // set state
  m_IsPreviewingSceneCamera = false;
  m_PreviewTarget = nullptr;
}
