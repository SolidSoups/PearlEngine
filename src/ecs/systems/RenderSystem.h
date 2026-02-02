#pragma once

#include "ecs_system.h"

// fwd
namespace ecs{
class Coordinator;
}
class Camera;
struct AmbientLightData;
class Scene;

// def
class RenderSystem : public ecs::System {
public:
  void Init(ecs::Coordinator* coordinator);
  void RenderAll();

private:
  ecs::Coordinator* mCoordinator;
};
