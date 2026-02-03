#pragma once

#include <memory>
#include "ecs_system.h"

namespace ecs{
class Coordinator;
}
struct ShaderData;

class PointLightSystem : public ecs::System{
public:
  void Init(ecs::Coordinator* coordinator);
  void SendUniforms(std::shared_ptr<ShaderData> shader);

private:
  ecs::Coordinator* mCoordinator;
};
