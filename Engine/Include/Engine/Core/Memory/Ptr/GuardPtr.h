#pragma once
#include <Engine/Core/Memory/STLAllocator.h>
template<typename T, typename _Alloc = STLAllocator<T>>
class GuardPtr {
public:
  using value_type = T;
  using pointer = T*;
  using void_pointer = void*;

  static constexpr size_t value_size = sizeof(T);
  static constexpr size_t used_align = alignof(T);

  template<typename... Args>
  GuardPtr(Args&&... args) {
    m_ptr = _allocator.allocate<used_align>(value_size);
    new (m_ptr) T(std::forward<Args>(args)...);
  }

  GuardPtr() {
    m_ptr = _allocator.allocate<used_align>(value_size);
    new (m_ptr) T();
  }

  ~GuardPtr() {
    if (m_ptr != nullptr)
    {
      reinterpret_cast<T*>(m_ptr)->~T();
      _allocator.deallocate<used_align>(m_ptr, value_size);

    }
  }

  pointer Get() {
    return reinterpret_cast<pointer>(m_ptr);
  }

private:
  void_pointer m_ptr;
  _Alloc _allocator;
};