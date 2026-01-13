#pragma once

#define ENABLE_BITMASK_OPERATORS(T)                                            \
inline T operator|(T a, T b) {                                               \
  return static_cast<T>(static_cast<int>(a) | static_cast<int>(b));          \
}                                                                            \
inline T operator&(T a, T b) {                                               \
  return static_cast<T>(static_cast<int>(a) & static_cast<int>(b));          \
}                                                                            \
inline T operator~(T a) { return static_cast<T>(~static_cast<int>(a)); }     \
inline T &operator|=(T &a, T b) { return a = a | b; }                        \
inline T &operator&=(T &a, T b) { return a = a & b; }
