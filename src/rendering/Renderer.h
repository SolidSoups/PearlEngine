#pragma once

#include "TransformComponent.h"
#include "RenderComponent.h"
class Camera;

class Renderer {
public:
  // Scene management
  static void BeginScene(Camera& camera);
  static void EndScene();

  static void Submit(const RenderComponent& renderComp, const TransformComponent& transformComp);

private:
  static Camera* s_ActiveCamera;
};
