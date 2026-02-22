#include "CameraSystem.h"

#include <glm/glm.hpp>

#include "CameraComponent.h"
// glm::mat4 CameraSystem::GetViewMatrix(TransformComponent& cmp){
//   return glm::lookAt(position, target, up);
// }
// glm::mat4 CameraSystem::GetProjectionMatrix(TransformComponent& cmp){
//   return glm::perspective(
//     glm::radians(fov),
//     aspectRatio,
//     nearPlane,
//     farPlane
//   );
// }
//


void CameraSystem::SetActiveCamera(ecs::Entity entity){
  mActiveCamera = entity; 
}

void CameraSystem::GetMatrices(CameraMode mode, glm::mat4 &outView, glm::mat4 &outProj){
  if(mode == ENGINE){
    outView = glm::lookAt(
      position, target, up 
    );
  }
}
