#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <typeindex>
#include <vector>

#include "Logger.h"
#include "ResourceManager.h"

class ResourceSystem {
private:
  std::unordered_map<std::type_index, std::unique_ptr<ResourceManagerBase>> 
    m_Managers;

public:
  ResourceSystem &operator=(const ResourceSystem &) = delete;

  // Create a resource and return a handle
  template<typename T>
  Handle<T> Create(T* data){
    return GetManager<T>().Create(data);
  }

  // Get a resource by handle
  template<typename T>
  T* Get(Handle<T> handle){
    return GetManager<T>().Get(handle);
  }

  // Destroy a resource by handle
  template<typename T>
  void Destroy(Handle<T> handle){
    GetManager<T>().Destroy(handle);
  }

  // Get the manager for a specific type
  template <typename T>
  ResourceManager<T>& GetManager(){
    auto typeIdx = std::type_index(typeid(T));

    if(m_Managers.find(typeIdx) == m_Managers.end()){
      m_Managers[typeIdx] = std::make_unique<ResourceManager<T>>();
    }

    return *static_cast<ResourceManager<T>*>(m_Managers[typeIdx].get());
  }

  template  <typename T>
  bool HasManager() {
    auto typeIdx = std::type_index(typeid(T));
    return m_Managers.find(typeIdx) != m_Managers.end();
  }

  void DestroyAllResources() {
    for(auto& [typeIdx, manager] : m_Managers){
      if(manager){
        manager->DestroyAll();
      }
    }
    m_Managers.clear();
  }
};
