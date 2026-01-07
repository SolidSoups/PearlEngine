#include "CameraController.h"
#include <glm/glm.hpp>
#include <stdatomic.h>

CameraController::CameraController(Camera* camera)
  : m_Camera(camera)
{
  Reset();
}

void CameraController::Reset(){
  m_Yaw = 90.0f;
  m_Pitch = 0.0f;
  m_OrbitDistance = 5.0f;
  UpdateCameraPosition();
  m_OrbitTarget = {0.0f, 0.0f, 0.0f};
  m_Camera->GetPreview();
}

void CameraController::OnUpdate(glm::vec2 mouseDelta, float scrollDelta, bool rightMouseDown, bool middleMouseDown){
  // we don't want to control a scene camera

  if(middleMouseDown){
    if(m_Camera->IsPreviewing()){
      m_Camera->StopPreview();
      RecalculateOrbitFromCamera();
    }
    Pan(mouseDelta);
    return;
  }

  if(rightMouseDown){
    if(m_Camera->IsPreviewing()){
      m_Camera->StopPreview();
      RecalculateOrbitFromCamera();
    }
    Orbit(mouseDelta);
  } 
  if(scrollDelta != 0){
    if(m_Camera->IsPreviewing()){
      m_Camera->StopPreview();
      RecalculateOrbitFromCamera();
    }
    Zoom(scrollDelta);
  }
}

void CameraController::RecalculateOrbitFromCamera(){
  auto& cam = m_Camera->GetInternal();

  // set the orbit target to where the camera is looking
  m_OrbitTarget = cam.target;

  // calculate orbit distance
  m_OrbitDistance = glm::length(cam.position - cam.target);

  // calculate new yaw and pitch from current cameras orientation
  glm::vec3 direction = glm::normalize(cam.position - cam.target);
  m_Pitch = glm::degrees(asin(direction.y));
  m_Yaw = glm::degrees(atan2(direction.z, direction.x));
}

void CameraController::Orbit(glm::vec2 delta){
  // Update yaw and pitch 
  m_Yaw += delta.x * m_OrbitSensitivity;
  m_Pitch += delta.y * m_OrbitSensitivity;

  // Clamp ptich to avoid gimbal locr
  m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);

  // Calculate new camera position using spherical coordinates
  float yawRad = glm::radians(m_Yaw);
  float pitchRad = glm::radians(m_Pitch);

  UpdateCameraPosition();
  m_Camera->GetInternal().target = m_OrbitTarget;
}

void CameraController::Zoom(float scrollDelta){
  m_OrbitDistance -= scrollDelta * m_ZoomSpeed;
  m_OrbitDistance = glm::clamp(m_OrbitDistance, 0.5f, 50.0f);
  UpdateCameraPosition();
}

void CameraController::UpdateCameraPosition(){
  float yawRad = glm::radians(m_Yaw);
  float pitchRad = glm::radians(m_Pitch);

  glm::vec3 position;
  position.x = m_OrbitTarget.x + m_OrbitDistance * cos(pitchRad) * cos(yawRad);
  position.y = m_OrbitTarget.y + m_OrbitDistance * sin(pitchRad);
  position.z = m_OrbitTarget.z + m_OrbitDistance * cos(pitchRad) * sin(yawRad);

  m_Camera->GetInternal().position = position;
}

void CameraController::Pan(glm::vec2 delta){
  // get camera right and up vectors
  auto& camera = m_Camera->GetInternal();
  glm::vec3 forward = camera.GetForward();
  glm::vec3 right = camera.GetRight();
  glm::vec3 up = camera.GetUp();

  // move camera and target
  glm::vec3 offset = (-right * delta.x + up * delta.y) * m_PanSensitivity * m_OrbitDistance;

  camera.position += offset;
  m_OrbitTarget += offset;
  camera.target = m_OrbitTarget;
}
