#pragma once

class Malloc {
public:
  virtual void* allocate(size_t bytes, size_t alignment) = 0;
  virtual void deallocate(void* ptr) noexcept = 0;
};
