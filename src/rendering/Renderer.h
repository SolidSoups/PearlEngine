#pragma once

#include "AmbientLightData.h"
#include "PointLightComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
class Camera;

class Renderer {
public:
  // Scene management
  static void BeginScene(Camera &camera, const AmbientLightData &ambientData);
  static void SubmitLights(const std::vector<PointLightComponent *> lights);
  static void EndScene();

  static void Submit(const RenderComponent &renderComp,
                     const TransformComponent &transformComp);
  static void SendLightUniforms(std::shared_ptr<ShaderData> shader);

private:
  static Camera *s_ActiveCamera;
  static AmbientLightData m_AmbientData;
  static std::vector<PointLightComponent *> m_PointLights;
  static bool m_bGeometryPassEnabled;
  static std::shared_ptr<ShaderData> m_NextShader;

public:
  static void SetGeometryPassEnabled(bool value);
  static void SetNextShader(std::shared_ptr<ShaderData> shader);
};
