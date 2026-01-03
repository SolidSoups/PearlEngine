#pragma once

#include <typeindex>
#include <unordered_map>
#include <any>
#include <cassert>
#include <vector>

#include "IServiceHandle.h"

class ServiceLocator {
public:
  template <typename T> void Provide(T *service) {
    m_Services[typeid(T)] = service;

    // see if any handles have requested this service (lazy references)
    if (IsReady<T>()) {
      for (auto *handle : m_Requests[typeid(T)]) {
        handle->ProvideService(service);
      }
      m_Requests.erase(typeid(T));
    }
  }

  template <typename T> T &Get() {
    auto it = m_Services.find(typeid(T));
    assert(it != m_Services.end() && "ServiceLocator: Service not registered");

    T *ptr = std::any_cast<T *>(it->second);
    assert(ptr != nullptr &&
           "ServiceLocator: Service registered but not ready");
    return *ptr;
  }

  template <typename T> void Reserve() {
    m_Services[typeid(T)] = static_cast<T *>(nullptr);
  }

  template <typename T> bool IsReady() const {
    auto it = m_Services.find(typeid(T));
    if (it == m_Services.end())
      return false;
    return std::any_cast<T *>(it->second) != nullptr;
  }

public:
  template <typename T> void RegisterRequest(IServiceHandle *handle) {
    if (IsReady<T>()) {
      T *val = std::any_cast<T *>(m_Services[typeid(T)]);
      handle->ProvideService(val);
      return;
    }
    m_Requests[typeid(T)].push_back(handle);
  }

private:
  std::unordered_map<std::type_index, std::vector<IServiceHandle *>> m_Requests;
  std::unordered_map<std::type_index, std::any> m_Services;
};
