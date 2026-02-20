#pragma once

#include "ecs_common.h"

class ComponentEditor{
private:
  bool m_HasInit = false;

public:

  ComponentEditor() = default;
  virtual ~ComponentEditor() = default;

  void SetInit() { m_HasInit = true; }
  bool HasInit() { return m_HasInit; }
public:
  virtual void OnInit(){}
  // will be called inside an imgui child window with typed component pointer
  // entity is provided for operations that need the entity ID
  virtual void OnDrawComponent(void* target, ecs::Entity entity) = 0;
  virtual const char* GetComponentName() const = 0;
  virtual const int GetSortOrder() const { return 0; }
};
