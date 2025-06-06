#pragma once
#include <Core/Memory/TrackingAllocator.hpp>

class MemoryManager {
public:
  template<typename T, typename... Args>
  static T* allocateObj(Args&&... args) {
    T* obj = new (std::nothrow) T(std::forward<Args>(args)...);
    return obj;
  }
  template <typename T>
  static void deallocateObj(T* obj) {
    delete obj;
  }
};
