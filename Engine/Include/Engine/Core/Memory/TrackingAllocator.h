#pragma once
#include <Include/Engine/Utils/WinInclude.h>
#include <vector>
#include <cstddef>
#include <new>
#include <type_traits>
#include <cassert>
#include <utility>

constexpr size_t BigAllocationThreshold = 1024 * 64;
constexpr size_t BigAllocationAlignment = 64;

class TrackingAllocator {
public:
  template <size_t Align>
  static void* allocate(size_t bytes) {
#if __cpp_lib_is_constant_evaluated
    if (std::is_constant_evaluated()) {
      return ::operator new(bytes); 
    }
#endif

#if defined(_M_IX86) || defined(_M_X64)
    size_t usedAlign = (bytes >= BigAllocationThreshold) ? std::max(Align, BigAllocationAlignment) : Align;
    void* ptr = ::operator new(bytes, std::align_val_t{ usedAlign });
#else
    void* ptr = ::operator new(bytes, std::align_val_t{ Align });
#endif

    m_totalAllocated += bytes;
    return ptr;
  }

  template <size_t Align>
  static void deallocate(void* ptr, size_t bytes) noexcept {
#if __cpp_lib_is_constant_evaluated
    if (std::is_constant_evaluated()) {
      ::operator delete(ptr); 
      return;
    }
#endif

#if defined(_M_IX86) || defined(_M_X64)
    size_t usedAlign = (bytes >= BigAllocationThreshold) ? std::max(Align, BigAllocationAlignment) : Align;
    ::operator delete(ptr, bytes, std::align_val_t{ usedAlign });
#else
    ::operator delete(ptr, bytes, std::align_val_t{ Align });
#endif

    m_totalAllocated -= bytes;
  }

  static std::size_t getTotalAllocated() {
    return m_totalAllocated;
  }

private:
  inline static std::size_t m_totalAllocated = 0;
};

extern TrackingAllocator* GMalloc = nullptr;

TrackingAllocator GInstance;
TrackingAllocator* GMalloc = &GInstance;