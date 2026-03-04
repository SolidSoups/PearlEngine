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

  inline const std::unordered_set<ecs::Entity>& GetRenderEntities() const {
    return Entities;
  }

private:
  ecs::Coordinator* mCoordinator;
};
