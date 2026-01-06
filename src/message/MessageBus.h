#pragma once

#include "IMessageHandler.h"
#include <cstddef>
#include <unordered_map>
#include <vector>
#include "Logger.h"
class MessageBus {
  std::unordered_map<size_t, std::vector<IMessageHandler*>> m_Handlers; 

public:
  // Subscribe an IMessageHandler to a message type
  template<typename T>
  void Subscribe(IMessageHandler* handler){
    m_Handlers[typeid(T).hash_code()].push_back(handler);
  }

  // Dispatch a message to all handlers
  void Dispatch(const Message& msg){
    auto it = m_Handlers.find(msg.GetTypeID());
    if(it != m_Handlers.end()){
      for(auto* handler : it->second){
        LOG_INFO << "Dispatching message to handler";
        handler->HandleMessage(msg);
      }
    }
  }
};


