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
  static void BeginScene(Camera &camera, const AmbientLightData &ambientData);
  static void SubmitLights(Scene& scene);
  static void EndScene();

  static void Submit(const RenderComponent &renderComp,
                     const TransformComponent &transformComp);
  static void SendLightUniforms(std::shared_ptr<ShaderData> shader);

private:
  static Camera *s_ActiveCamera;
  static AmbientLightData m_AmbientData;
  static std::vector<ecs::Entity> m_PointLightEntities;
  static Scene* m_Scene;
  static bool m_bGeometryPassEnabled;
  static std::shared_ptr<ShaderData> m_NextShader;

public:
  static void SetGeometryPassEnabled(bool value);
  static void SetNextShader(std::shared_ptr<ShaderData> shader);
};
