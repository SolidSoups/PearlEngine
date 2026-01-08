#pragma once

#include <functional>
#include <cstdint>

// Resource Handle ID
using HandleID = uint32_t;
// Resource Generation ID
using HandleGenID = uint32_t;

template<typename T>
struct Handle{ 
  HandleID id;   
  HandleGenID generation;

  bool operator==(const Handle& other) const {
    return id == other.id && generation == other.generation;
  };
  bool operator!=(const Handle& other) const {
    return id != other.id || generation != other.generation;
  }
  bool IsValid(){
    return id != 0;
  }
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
