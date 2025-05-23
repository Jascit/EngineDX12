#pragma once
#include <Engine/Core/Systems/MemoryManagmentSystem/MemoryManagmentSystem.h>
#include <Engine/Core/memory/TrackingAllocator.h>
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

  template<typename U>
  STLAllocator(const STLAllocator<U>&) noexcept {}
  STLAllocator() {}

  pointer allocate(size_type n) {
    if (n == 0) return nullptr;
    LLM_SCOPE_BYTAG(LLMTags::STD)
    size_t bytes = n * sizeof(T);
    void* p = GMalloc->allocate(bytes, alignof(T));
    return static_cast<T*>(p);
  }

  void deallocate(pointer p, size_type n) noexcept {
    LLM_SCOPE_BYTAG(LLMTags::STD)
    GMalloc->deallocate(p);
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
};

template<typename T>
using tracked_vector = std::vector<T, STLAllocator<T>>;
template<typename KTy, typename Ty>
using tracked_unordered_map = std::unordered_map<KTy, Ty, std::hash<KTy>, std::equal_to<KTy>, STLAllocator<std::pair<const KTy, Ty>>>;
