#pragma once

#include "Component.h"
#include "PointLightData.h"
#include <memory>

class PointLightComponent : public IComponent{
  COMPONENT_CLASS(PointLightComponent)

public:
  PointLightData data;
};
