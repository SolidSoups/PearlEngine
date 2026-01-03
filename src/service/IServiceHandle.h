#pragma once

class IServiceHandle {
public:
  virtual void ProvideService(void* service) = 0;
};
