#pragma once

#include <cstddef>
#include <unordered_map>
#include <vector>

#include "IMessageHandler.h"
#include "Logger.h"

/** Dispatches messages to subscribers immediately. Can be used for immediate
 * notification, or in combination with MessageQueue for deferred notification.
 */
class MessageBus {
  std::unordered_map<size_t, std::vector<IMessageHandler *>> m_Handlers;

public:
  // Subscribe an IMessageHandler to a message type
  template <typename T> void Subscribe(IMessageHandler *handler) {
    m_Handlers[typeid(T).hash_code()].push_back(handler);
  }

  // Dispatch a message to all handlers
  void Dispatch(const Message &msg) {
    auto it = m_Handlers.find(msg.GetTypeID());
    if (it != m_Handlers.end()) {
      for (auto *handler : it->second) {
        LOG_INFO << "Dispatching message to handler";
        handler->HandleMessage(msg);
      }
    }
  }
};
