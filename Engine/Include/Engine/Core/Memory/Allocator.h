#pragma once
#include <Include/Engine/Core/Memory/GMalloc.h>
//TODO:
template<typename T>
class Allocator {
//not magnifying data
public:
  T** allocate(size_t count) {
    m_data = GMalloc::malloc(count * sizeof(T));
    return m_data;
  }
  void deallocate() {
    if (m_data)
    {
      GMalloc::free(m_data);
      m_data = nullptr;
    }
  }
  
private:
  T* m_data;
  size_t _size;
};