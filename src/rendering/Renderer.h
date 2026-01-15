#pragma once

#include "AmbientLightData.h"
#include "PointLightComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
class Camera;

class Renderer {
public:
  // Scene management
  static void BeginScene(Camera& camera, const AmbientLightData& ambientData);
  static void SubmitLights(const std::vector<PointLightComponent*> lights);
  static void EndScene();

  static void Submit(
    const RenderComponent& renderComp, const TransformComponent& transformComp);

private:
  static Camera* s_ActiveCamera;
  static AmbientLightData m_AmbientData;
  static std::vector<PointLightComponent*> m_PointLights;
};
