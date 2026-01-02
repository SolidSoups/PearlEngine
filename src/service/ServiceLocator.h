#pragma once

#include <typeindex>
#include <unordered_map>
#include <any>
#include <cassert>

class ServiceLocator {
public:
  template<typename T>
  void Provide(T* service){
    m_Services[typeid(T)] = service;
  }

  template<typename T>
  T& Get(){
    auto it = m_Services.find(typeid(T));
    assert(it != m_Services.end() && "ServiceLocator: Service not registered");

    T* ptr = std::any_cast<T*>(it->second);
    assert(ptr != nullptr && "ServiceLocator: Service registered but not ready");
    return *ptr;
  }

  template<typename T>
  void Reserve(){
    m_Services[typeid(T)] = static_cast<T*>(nullptr);
  }

  template<typename T>
  bool IsReady() const {
    auto it = m_Services.find(typeid(T));
    if(it == m_Services.end()) return false;
    return std::any_cast<T*>(it->second) != nullptr;
  }

private:
  std::unordered_map<std::type_index, std::any> m_Services;
};
