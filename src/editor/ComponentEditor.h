#pragma once

#include "Component.h"


class ComponentEditor{
public:
  ComponentEditor() = default;
  virtual ~ComponentEditor() = default;

public:

  virtual void OnInit(){}
  // will be called inside an imgui child window
  virtual void OnDrawComponent(IComponent* target) = 0;
  virtual const int GetSortOrder() const { return 0; }
};
