#include "Camera.h"
#include "ServiceLocator.h"
#include "CameraComponent.h"

Camera::Camera()
  : r_Scene(){}

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

void Camera::SyncFromComponent(){
  CameraComponent* activeCam = r_Scene->GetActiveCamera();
  if(!activeCam) return;

  // pull data from component
  m_CameraData = activeCam->cameraData;

  // TODO: get the owner and set properties from position and rotatation

  m_CameraData.UpdateCameraVectors();
}

void Camera::Update(){
  SyncFromComponent();
}
