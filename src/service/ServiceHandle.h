#pragma once

#include "ServiceLocator.h"
#include "IServiceHandle.h"
#include <cstdint>

enum ServiceHandleState : uint8_t {
    Unregistered = 0,
    Registered,
    Ready,
    Failed
};

template <typename T> class ServiceHandle : public IServiceHandle {
  public:
    ServiceHandle() {
        ServiceLocator::RegisterRequest<T>(this);
        m_State = Registered;
    }

    ServiceHandleState const GetState() const { return m_State; }
  bool const IsReady() const { return m_State == Ready; }

  public:
    void ProvideService(void *service) override {
        value = static_cast<T *>(service);
        if (value)
            m_State = Ready;
        else
            m_State = Failed;
    }

  private:
    T *value = nullptr;
    ServiceHandleState m_State = Unregistered;

  public:
    T &operator*() {
        assert(value != nullptr && "ServiceHandle: Service not ready");
        return *value;
    }
    T &operator*() const {
        assert(value != nullptr && "ServiceHandle: Service not ready");
        return *value;
    }
    T *operator->() {
        assert(value != nullptr && "ServiceHandle: Service not ready");
        return value;
    }
    T *operator->() const {
        assert(value != nullptr && "ServiceHandle: Service not ready");
        return value;
    }
    T *Get() { return value; }
    T *Get() const { return value; }
};
