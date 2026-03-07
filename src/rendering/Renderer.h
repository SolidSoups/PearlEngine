#pragma once

#include "AmbientLightData.h"
#include "PointLightComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ecs_common.h"
#include <vector>

class Camera;
class Scene;

class Renderer {
public:
  // Scene management
  static void BeginScene(Camera &camera);
  static void BeginScene(glm::mat4 view, glm::mat4 proj);
  static void SubmitLights(Scene &scene);
  static void EndScene();

  static void Submit(const RenderComponent &renderComp,
                     const TransformComponent &transformComp);
  static void Submit(const TransformComponent& aTransform, const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);
  static void SendLightUniforms(std::shared_ptr<ShaderData> shader);
  static void SendAmbientLightUniforms(std::shared_ptr<ShaderData> shader);

  // returns false on failure
  static bool ResolveCamera(glm::mat4& view, glm::mat4& proj);

private:
  static Camera *s_ActiveCamera;
  static glm::mat4 s_View;
  static glm::mat4 s_Proj;
  static AmbientLightData m_AmbientData;
  static std::vector<ecs::Entity> m_PointLightEntities;
  static Scene *m_Scene;
  static bool m_bGeometryPassEnabled;
  static std::shared_ptr<ShaderData> m_NextShader;

public:
  static void SetGeometryPassEnabled(bool value);
  static void SetNextShader(std::shared_ptr<ShaderData> shader);
};
