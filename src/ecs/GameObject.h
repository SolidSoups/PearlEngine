#pragma once

#include <typeindex>
#include <unordered_map>
#include <memory>

#include "ecs_common.h"
#include "Component.h"

class GameObject{
  EntityID m_ID;
  std::unordered_map<std::type_index, std::unique_ptr<Component>> m_Components;

public:
  GameObject(EntityID id) : m_ID(id) {}

  template<typename T, typename ... Args>
  T* AddComponent(Args&&... args){
    auto component = std::make_unique<T>(std::forward<Args>(args)...);
    T* ptr = component.get();
    component->owner = m_ID;
    m_Components[typeid(T)] = std::move(component);
    return ptr;
  }

  template<typename T>
  T* GetComponent(){
    auto it = m_Components.find(typeid(T));
    if(it != m_Components.end()){
      return static_cast<T*>(it->second.get());
    }
    return nullptr;
  }

  template<typename T>
  void RemoveComponent(){
    m_Components.erase(typeid(T));
  }

  const auto& GetAllComponents() const {
    return m_Components;
  }

  template<typename T>
  bool HasComponent() const {
    return m_Components.find(typeid(T)) != m_Components.end();
  }

  EntityID GetID() const { return m_ID; }
};
