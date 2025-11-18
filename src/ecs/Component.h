#pragma once

#include <typeindex>

#include "ecs_common.h"


class Component{
public:
  virtual ~Component() = default;
  virtual const char* GetComponentName() const = 0;
  virtual std::type_index GetTypeIndex() const = 0;

public:
  EntityID owner = 0;
};

#define COMPONENT_CLASS(ClassName) \
public: \
  std::type_index GetTypeIndex() const override { \
    return std::type_index(typeid(ClassName)); \
  } \
  const char* GetComponentName() const override { return #ClassName; }
