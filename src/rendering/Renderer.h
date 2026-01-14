#pragma once

#include "AmbientLightData.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
class Camera;

class Renderer {
public:
  // Scene management
  static void BeginScene(Camera& camera);
  static void EndScene();

  static void Submit(
    const RenderComponent& renderComp, const TransformComponent& transformComp,
    const AmbientLightData& ambientData);

private:
  static Camera* s_ActiveCamera;
};
