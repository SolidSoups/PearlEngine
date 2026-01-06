#pragma once

#include "TransformComponent.h"
#include "RenderComponent.h"
class Camera;
class ResourceSystem;

class Renderer {
public:
  // Scene management
  static void BeginScene(Camera& camera);
  static void EndScene();

  static void Submit(ResourceSystem* rs, const RenderComponent& renderComp, const TransformComponent& transformComp);

private:
  static Camera* s_ActiveCamera;
};
