#include "CameraController.h"
#include <glm/glm.hpp>

CameraController::CameraController(Camera* camera)
  : m_Camera(camera)
{
  // Calculate initial yaw/pitch from camera position
  glm::vec3 direction = glm::normalize(m_OrbitTarget - m_Camera->GetPosition());
  m_Pitch = glm::degrees(asin(direction.y));
  m_Yaw = glm::degrees(atan2(direction.z, direction.x));
}

void CameraController::Reset(){
  m_Yaw = 90.0f;
  m_Pitch = 0.0f;
  m_OrbitDistance = 5.0f;
  UpdateCameraPosition();
}

void CameraController::OnUpdate(glm::vec2 mouseDelta, float scrollDelta, bool rightMouseDown, bool middleMouseDown){
  if(middleMouseDown){
    Pan(mouseDelta);
    return;
  }

  if(rightMouseDown){
    Orbit(mouseDelta);
  } 
  if(scrollDelta != 0){
    Zoom(scrollDelta);
  }
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
  m_Camera->SetTarget(m_OrbitTarget);
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

  m_Camera->SetPosition(position);
}

void CameraController::Pan(glm::vec2 delta){
  // get camera right and up vectors
  glm::vec3 forward = m_Camera->GetForward();
  glm::vec3 right = m_Camera->GetRight();
  glm::vec3 up = m_Camera->GetUp();

  // move camera and target
  glm::vec3 offset = (-right * delta.x + up * delta.y) * m_PanSensitivity * m_OrbitDistance;

  glm::vec3 newPos = m_Camera->GetPosition() + offset;
  m_Camera->SetPosition(newPos);
  m_OrbitTarget += offset;
  m_Camera->SetTarget(m_OrbitTarget);
}
