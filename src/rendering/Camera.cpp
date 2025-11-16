#include "Camera.h"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/geometric.hpp"
#include <iostream>
#include "debug_extensions.h"

#include "Logger.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
  : m_Position(position)
  , m_Target(target)
  , m_WorldUp(worldUp)
  , m_FOV(glm::radians(60.0f))
  , m_AspectRatio(1.0f)
  , m_NearPlane(0.1f)
  , m_FarPlane(100.0f)
{
  UpdateCameraVectors();
  OutputParameters();
}

glm::mat4 Camera::GetViewMatrix() const{
  return glm::lookAt(m_Position, m_Target, m_Up);
}

glm::mat4 Camera::GetProjectionMatrix() const {
  return glm::perspective(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
}

void Camera::SetPosition(glm::vec3& position){
  m_Position = position;
  UpdateCameraVectors();
}

void Camera::SetTarget(glm::vec3& target){
  m_Target = target;
  UpdateCameraVectors();
}

void Camera::SetAspectRatio(float aspectRatio){
  m_AspectRatio = aspectRatio;
  UpdateCameraVectors();
}

void Camera::Translate(glm::vec3& offset){
  m_Position += offset;
  m_Target += offset;
  UpdateCameraVectors();
}

void Camera::Move(float forward, float right, float up){
  m_Position += m_Forward * forward;
  m_Position += m_Right * right;
  m_Position += m_Up * up;
  m_Target += m_Forward * forward;
  m_Target += m_Right * right;
  m_Target += m_Up * up;
  UpdateCameraVectors();
}

void Camera::UpdateCameraVectors(){
  m_Forward = glm::normalize(m_Target - m_Position);
  m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));
  m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
}

void Camera::OutputParameters() const{
  LOG_INFO << "Camera::OutputParameters() -> params = {\n"
      << "\t\tPosition: " << m_Position << ",\n"
      << "\t\tTarget: " << m_Target << ",\n"
      << "\t\tWorldUp: " << m_WorldUp << ",\n"
      << "\t\tForward: " << m_Forward << ",\n"
      << "\t\tRight: " << m_Right << ",\n"
      << "\t\tUp: " << m_Up << ",\n"
      << "\t\tFOV: " << m_FOV << ",\n"
      << "\t\tAspectRatio: " << m_AspectRatio << ",\n"
      << "\t\tNear Plane: " << m_NearPlane << ",\n"
      << "\t\tFar Plane: " << m_FarPlane << ",\n"
    << "}";
}

