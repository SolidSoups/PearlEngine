#pragma once

#include <deque>
#include <mutex>

#include "Message.h"

/** Stores messages in a thread-safe queue for deferred notificaiton.
 *  Is used exclusively with MessageBus internally to defer notifications
  */
class MessageQueue {
  std::deque<Message> m_Queue;
  std::mutex mutex;

public:
  // Post a message to the queue
  template <typename T> void Dispatch(const T &msg) {
    std::lock_guard lock(mutex);
    m_Queue.push_back(Message::Create(msg));
  }

  // Get all messages. Swaps the queue and returns it. Doesn't block mutex.
  std::deque<Message> DrainAll() {
    std::deque<Message> local;
    {
      std::lock_guard lock(mutex);
      local.swap(m_Queue);
    }
    return local;
  }
};
