#pragma once
//all base alocators(without templates) must use this template and allocate bytes
class IAllocator {
  virtual void* allocate(size_t bytes, size_t alignment) = 0;
  virtual void  deallocate(void* ptr, size_t bytes, size_t alignment) = 0;
  virtual ~IAllocator() = default;
};