#include "Camera.h"
#include "ServiceLocator.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/quaternion.hpp"

Camera::Camera() : r_Scene() {}

void Camera::StartPreview(ecs::Entity cameraEntity) {
  m_IsPreviewingSceneCamera = true;
  m_PreviewTarget = cameraEntity;
}

void Camera::SetAspect(float aspect) {
  m_Aspect = aspect;
  m_InternalCameraData.aspectRatio = aspect;
  if (m_PreviewTarget != ecs::NULL_ENTITY && r_Scene.GetState() == Ready) {
    auto& coordinator = r_Scene->GetCoordinator();
    if (coordinator.HasComponent<CameraComponent>(m_PreviewTarget)) {
      coordinator.GetComponent<CameraComponent>(m_PreviewTarget).cameraData.aspectRatio = aspect;
    }
  }
}

CameraData* Camera::GetPreview(){
  if(m_IsPreviewingSceneCamera && m_PreviewTarget != ecs::NULL_ENTITY && r_Scene.GetState() == Ready){
    auto& coordinator = r_Scene->GetCoordinator();
    if (!coordinator.HasComponent<CameraComponent>(m_PreviewTarget)) return nullptr;
    if (!coordinator.HasComponent<TransformComponent>(m_PreviewTarget)) return nullptr;

    auto& camComp = coordinator.GetComponent<CameraComponent>(m_PreviewTarget);
    auto& transform = coordinator.GetComponent<TransformComponent>(m_PreviewTarget);

    // update position
    camComp.cameraData.position = transform.position;

    // build rotation matrix from euler angles
    glm::quat orientation = glm::quat(glm::vec3(
        glm::radians(transform.rotation.x),
        glm::radians(transform.rotation.y),
        glm::radians(transform.rotation.z)
    ));

    // extract forward and up from matrix
    glm::vec3 forward = orientation * glm::vec3(0, 0, -1); // Local -Z
    glm::vec3 up = orientation * glm::vec3(0, 1, 0); // Local y

    // update camera vectors to match
    camComp.cameraData.target = transform.position + forward;
    camComp.cameraData.worldUp = up;
    camComp.cameraData.UpdateCameraVectors();

    camComp.cameraData.aspectRatio = m_Aspect;
    return &camComp.cameraData;
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
  if(m_IsPreviewingSceneCamera && m_PreviewTarget != ecs::NULL_ENTITY && r_Scene.GetState() == Ready){
    auto& coordinator = r_Scene->GetCoordinator();
    if (coordinator.HasComponent<CameraComponent>(m_PreviewTarget)) {
      m_InternalCameraData = coordinator.GetComponent<CameraComponent>(m_PreviewTarget).cameraData;
    }
  }

  // set state
  m_IsPreviewingSceneCamera = false;
  m_PreviewTarget = ecs::NULL_ENTITY;
}
