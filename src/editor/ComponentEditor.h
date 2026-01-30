#pragma once

#include "ecs_common.h"

class ComponentEditor{
public:
  ComponentEditor() = default;
  virtual ~ComponentEditor() = default;

public:

  virtual void OnInit(){}
  // will be called inside an imgui child window with typed component pointer
  // entity is provided for operations that need the entity ID
  virtual void OnDrawComponent(void* target, ecs::Entity entity) = 0;
  virtual const char* GetComponentName() const = 0;
  virtual const int GetSortOrder() const { return 0; }
};
