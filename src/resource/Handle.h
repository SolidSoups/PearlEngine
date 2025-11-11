#pragma once

#include "ResourceCommon.h"

template<typename T>
struct Handle{ 
  HandleID id;   
  HandleGenID generation;

  bool operator==(const Handle& other) const {
    return id == other.id && generation == other.generation;
  };
  static constexpr Handle Invalid() {
    return {0, 0};
  };

  // add hashing to use unordered maps
};

// TODO: convert function to handle bit overflow in case of 32-bit systems (why would i ever port to 32 bit systems)
template<typename T>
struct std::hash<Handle<T>> {
  size_t operator()(const Handle<T>& h) const {
    return (size_t(h.id) << 32) | h.generation;
  }
};
