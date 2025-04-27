#pragma once

class Malloc {
public:
  virtual void* allocate(uint32_t bytes, uint32_t alignment) = 0;
  virtual void deallocate(void* ptr) noexcept = 0;
};
