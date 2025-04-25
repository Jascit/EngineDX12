#pragma once
#include <Include/Engine/Core/Systems/MemoryManagmentSystem/MemoryManagentSystem.h>
#include <Include/Engine/Core/memory/DefaultAllocator.h>
#include <memory>
#include <cstddef>
#include <new>
#include <type_traits>

template<typename T>
class STLAllocator {
public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using void_pointer = void*;
  using const_void_pointer = const void*;
  using size_type = _STD size_t;
  using difference_type = _STD ptrdiff_t;

  template<typename U>
  struct rebind {
    using other = STLAllocator<U>;
  };

  STLAllocator(const std::string& name)
    : _name(name)
  {
    _alloc = MemoryManagentSystem::instance().findAllocator(_name);
    if (!_allocator) throw std::runtime_error("Allocator not found");
  }

  template<typename U>
  STLAllocator(const STLAllocator<U>&) noexcept {}

  pointer allocate(size_type n) {
    if (n == 0) return nullptr;
    size_t bytes = n * sizeof(T);
    void* p = allocator->allocate(bytes, alignof(T));
    return static_cast<T*>(p);
  }

  void deallocate(pointer p, size_type n) noexcept {
    if (p == nullptr) return;
    size_t bytes = n * sizeof(T);
    _alloc->deallocate(p, bytes);
  }

  template<typename U, typename... Args>
  void construct(U* p, Args&&... args) {
    ::new(static_cast<void*>(p)) U(std::forward<Args>(args)...);
  }

  template<typename U>
  void destroy(U* p) {
    p->~U();
  }

  bool operator==(const STLAllocator&) const noexcept { return true; }
  bool operator!=(const STLAllocator&) const noexcept { return false; }

private:
  std::string _name;
  IAllocator* _alloc;
};

