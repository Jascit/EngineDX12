#pragma once
#include <Windows.h>
#include <type_traits>
#include <minmax.h>
//TODO: that's a fucking bullshit, just kill yourself
constexpr size_t BigAllocationThreshold = 1024 * 64;
constexpr size_t BigAllocationAlignment = 64;

class Memory {
public:
  static void* allocate(size_t bytes, size_t alignment) {
#if __cpp_lib_is_constant_evaluated
    if (std::is_constant_evaluated()) {
      return ::operator new(bytes);
    }
#endif

#if defined(_M_IX86) || defined(_M_X64)
    size_t usedAlign = max(alignment, BigAllocationAlignment);
    void* ptr = ::operator new(bytes, std::align_val_t{ usedAlign });
#else
    void* ptr = ::operator new(bytes, std::align_val_t{ alignment });
#endif

    return ptr;
  }

  static void deallocate(void* ptr, size_t alignment) noexcept {
#if __cpp_lib_is_constant_evaluated
    if (std::is_constant_evaluated()) {
      ::operator delete(ptr);
      return;
    }
#endif

#if defined(_M_IX86) || defined(_M_X64)
    size_t usedAlign = max(alignment, BigAllocationAlignment);
    ::operator delete(ptr, std::align_val_t{ usedAlign });
#else
    ::operator delete(ptr, std::align_val_t{ alignment });
#endif

  }
public:
  static void* allocate(size_t bytes, size_t alignment) {
    size_t total = bytes + ((alignment - 1 + sizeof(Metadata) & ~alignment));
    void* raw = ::operator new(total, std::align_val_t{ alignment });
    uintptr_t rawAddr = reinterpret_cast<uintptr_t>(raw) + sizeof(Metadata);
    uintptr_t alignedAddr = (rawAddr + alignment - 1) & ~alignment;
    void* aligned = reinterpret_cast<void*>(alignedAddr);

    Metadata* meta = reinterpret_cast<Metadata*>(aligned) - 1;
    meta->original = raw;
    meta->alignment = alignment;

    return aligned;
  }

  static void deallocate(void* ptr) noexcept {
    if (!ptr) return;

    Metadata* meta = reinterpret_cast<Metadata*>(ptr) - 1;
    void* raw = meta->original;
    size_t alignment = meta->alignment;

    ::operator delete(raw, std::align_val_t{ alignment });
  }

private:
  struct Metadata {
    void* original;
    size_t alignment;
  };
};