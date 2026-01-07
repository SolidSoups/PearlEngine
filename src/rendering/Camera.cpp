#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
  m_CameraData.position = position;
  m_CameraData.target = target;
  m_CameraData.worldUp = worldUp;
  m_CameraData.fov = 60.0f;
  m_CameraData.aspectRatio = 1.0f;
  m_CameraData.nearPlane = 0.1f;
  m_CameraData.farPlane = 100.0f;
  m_CameraData.UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const{
  return m_CameraData.GetViewMatrix();
}

glm::mat4 Camera::GetProjectionMatrix() const {
  return m_CameraData.GetProjectionMatrix();
}

void Camera::SetPosition(glm::vec3& position){
  m_CameraData.position = position;
  m_CameraData.UpdateCameraVectors();
}

void Camera::SetTarget(glm::vec3& target){
  m_CameraData.target = target;
  m_CameraData.UpdateCameraVectors();
}

void Camera::SetAspectRatio(float aspectRatio){
  m_CameraData.aspectRatio = aspectRatio;
  m_CameraData.UpdateCameraVectors();
}

void Camera::Translate(glm::vec3& offset){
  m_CameraData.position += offset;
  m_CameraData.target += offset;
  m_CameraData.UpdateCameraVectors();
}

void Camera::Move(float forward, float right, float up){
  m_CameraData.position += m_CameraData.GetForward() * forward;
  m_CameraData.position += m_CameraData.GetRight() * right;
  m_CameraData.position += m_CameraData.GetUp() * up;
  m_CameraData.target += m_CameraData.GetForward() * forward;
  m_CameraData.target += m_CameraData.GetRight() * right;
  m_CameraData.target += m_CameraData.GetUp() * up;
  m_CameraData.UpdateCameraVectors();
}
