#pragma once

#include "Message.h"
class IMessageHandler {
public:
  virtual ~IMessageHandler() = default;
  virtual void HandleMessage(const Message& msg) = 0;
};
