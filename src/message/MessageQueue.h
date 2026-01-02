#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <utility>
#include <any>
#include <functional>

class MessageQueue {
  std::unordered_map<std::type_index, std::vector<std::any>> m_Messages;
  using GenericMessageHandler = std::function<void(const std::any &)>;
  std::unordered_map<std::type_index, std::vector<GenericMessageHandler>> m_Handlers;

public:
  template <typename T> void post(T &&message) {
    // two notes here:
    //  1) std::decay_t<T> will strip away all modifiers such as & and &&
    //  references, const/volatile, arrays become pointers, functions become
    //  function pointers
    //  2) std::forward will pass along as is. if l-value: copy, if r-value: move
    m_Messages[std::type_index(typeid(std::decay_t<T>))].push_back(std::forward<T>(message));
  }

  template <typename T> void subscribe(std::function<void(const T&)> handler) {
    auto wrapper = [handler](const std::any& msg){
      const T& data = std::any_cast<const T&>(msg);
      handler(data);
    };
    m_Handlers[std::type_index(typeid(T))].push_back(wrapper);
  }

  void dispatch(){
    for(auto& [type, messages] : m_Messages){
      if(m_Handlers.count(type) == 0) continue;

      for(auto& msg : messages){
        for(auto& handler : m_Handlers[type]){
          handler(msg);
        }
      }
    }
    m_Messages.clear();
  }
};
