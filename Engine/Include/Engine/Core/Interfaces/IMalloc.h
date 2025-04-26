#pragma once
// Maximum alignment to reserve space for metadata
constexpr size_t MaxAllocationAlignment = 64;
// Default alignment if none is specified
constexpr size_t DefaultAllocationAlignment = 16;

class Malloc {
public:
  virtual void* allocate(size_t bytes, size_t alignment = DefaultAllocationAlignment) = 0;
  virtual void deallocate(void* ptr) noexcept = 0;
};
