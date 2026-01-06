#pragma once
#include <cstddef>
#include <any>

class Message {
  size_t m_TypeID;
  std::any m_Data;

public:
  template <typename T> static Message Create(const T &msg) {
    Message m;
    m.m_TypeID = typeid(T).hash_code();
    m.m_Data = std::any(msg);
    return m;
  }

  template <typename T> const T *As() const {
    if (m_TypeID == typeid(T).hash_code()) {
      return std::any_cast<T>(&m_Data);
    }
    return nullptr;
  }

  size_t const GetTypeID() const { return m_TypeID; }
};
