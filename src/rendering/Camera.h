#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera{
public:
  Camera(
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f));

  // Get the view matrix calculated using lookAt
  glm::mat4 GetViewMatrix() const;

  // Get the projection matrix
  glm::mat4 GetProjectionMatrix() const;

  // setters 
  void SetPosition(glm::vec3& position);
  void SetTarget(glm::vec3& target);
  void SetAspectRatio(float& aspectRatio);

  // getters
  glm::vec3 GetPosition() const { return m_Position; }
  glm::vec3 GetTarget() const { return m_Target; }
  glm::vec3 GetForward() const { return m_Forward; }
  glm::vec3 GetRight() const { return m_Right; }
  glm::vec3 GetUp() const { return m_Up; }
  float GetAspectRatio() const { return m_AspectRatio; }

  // Movement
  void Translate(glm::vec3& offset);
  void Move(float forward, float right, float up);

private:
  // Camera position and orientation
  glm::vec3 m_Position;
  glm::vec3 m_Target;
  glm::vec3 m_WorldUp;

  // Camera basis vectors
  glm::vec3 m_Forward;
  glm::vec3 m_Right;
  glm::vec3 m_Up;

  // Perspective parameters
  float m_FOV;
  float m_AspectRatio;

  // Frustrum parameters
  float m_NearPlane;
  float m_FarPlane;

  // internal update camera vectors
  void UpdateCameraVectors();

public: // DEBUG
  void OutputParameters() const;
};
