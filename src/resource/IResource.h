#pragma once

#include <typeindex>
class IResource {
  public:
    virtual ~IResource() = default;

  public:
    virtual std::type_index GetTypeIndex() const = 0;
};

#define RESOURCE_CLASS(ResourceType)                                           \
  public:                                                                      \
    std::type_index GetTypeIndex() const override {                            \
        return std::type_index(typeid(ResourceType));                          \
    }
