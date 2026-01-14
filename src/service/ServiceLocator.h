#pragma once

#include <typeindex>
#include <unordered_map>
#include <any>
#include <cassert>
#include <vector>
#include "Logger.h"

#include "IServiceHandle.h"

// NEVER, ABSOLUTELY NEVER ALLOWED TO OWN DATA
class ServiceLocator {
public:
  template <typename T> static void Provide(T *service) {
    auto& instance = GetInstance();
    if(instance.m_Services.find(typeid(T)) != instance.m_Services.end()){
      LOG_WARNING << "Service of type " << typeid(T).name() << " already exists!";
    }

    instance.m_Services[typeid(T)] = service;

    // see if any handles have requested this service (lazy references)
    if (IsReady<T>()) {
      for (auto *handle : instance.m_Requests[typeid(T)]) {
        handle->ProvideService(service);
      }
      instance.m_Requests.erase(typeid(T));
    }
  }

  template <typename T> static T &Get() {
    auto& instance = GetInstance();
    auto it = instance.m_Services.find(typeid(T));
    assert(it != instance.m_Services.end() && "ServiceLocator: Service not registered");

    T *ptr = std::any_cast<T *>(it->second);
    assert(ptr != nullptr &&
           "ServiceLocator: Service registered but not ready");
    return *ptr;
  }

  template <typename T> static void Reserve() {
    auto& instance = GetInstance();
    instance.m_Services[typeid(T)] = static_cast<T *>(nullptr);
  }

  template <typename T> static bool IsReady() {
    auto& instance = GetInstance();
    auto it = instance.m_Services.find(typeid(T));
    if (it == instance.m_Services.end())
      return false;
    return std::any_cast<T *>(it->second) != nullptr;
  }

  template <typename T> static void RegisterRequest(IServiceHandle *handle) {
    auto& instance = GetInstance();
    if (IsReady<T>()) {
      T *val = std::any_cast<T *>(instance.m_Services[typeid(T)]);
      handle->ProvideService(val);
      return;
    }
    instance.m_Requests[typeid(T)].push_back(handle);
  }

  static void Destroy() {
    auto& instance = GetInstance();
    instance.m_Services.clear();
    instance.m_Requests.clear();
  }

private:
  // Private constructor for singleton
  ServiceLocator() = default;

  // Delete copy and move
  ServiceLocator(const ServiceLocator&) = delete;
  ServiceLocator& operator=(const ServiceLocator&) = delete;
  ServiceLocator(ServiceLocator&&) = delete;
  ServiceLocator& operator=(ServiceLocator&&) = delete;

  // Meyer's singleton - thread-safe in C++11+
  static ServiceLocator& GetInstance() {
    static ServiceLocator instance;
    return instance;
  }

  std::unordered_map<std::type_index, std::vector<IServiceHandle *>> m_Requests;
  std::unordered_map<std::type_index, std::any> m_Services;
};
