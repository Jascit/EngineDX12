#pragma once
#include <Include/Engine/Core/Interfaces/IAllocator.h>
#include <Include/Engine/Utils/WinInclude.h>
#include <vector>
#include <cstddef>
#include <new>
#include <type_traits>
#include <cassert>
#include <utility>

constexpr size_t BigAllocationThreshold = 1024 * 64;
constexpr size_t BigAllocationAlignment = 64;

class DefaultAllocator : IAllocator {
public:
  void* allocate(size_t bytes, size_t alignment) override {
#if __cpp_lib_is_constant_evaluated
    if (std::is_constant_evaluated()) {
      return ::operator new(bytes);
    }
#endif

#if defined(_M_IX86) || defined(_M_X64)
    size_t usedAlign = (bytes >= BigAllocationThreshold) ? std::max(alignment, BigAllocationAlignment) : alignment;
    void* ptr = ::operator new(bytes, std::align_val_t{ usedAlign });
#else
    void* ptr = ::operator new(bytes, std::align_val_t{ Align });
#endif

    m_totalAllocated += bytes;
    return ptr;
  }

  void deallocate(void* ptr, size_t bytes, size_t alignment) noexcept {
#if __cpp_lib_is_constant_evaluated
    if (std::is_constant_evaluated()) {
      ::operator delete(ptr);
      return;
    }
#endif

#if defined(_M_IX86) || defined(_M_X64)
    size_t usedAlign = (bytes >= BigAllocationThreshold) ? std::max(alignment, BigAllocationAlignment) : alignment;
    ::operator delete(ptr, bytes, std::align_val_t{ usedAlign });
#else
    ::operator delete(ptr, bytes, std::align_val_t{ Align });
#endif

    m_totalAllocated -= bytes;
  }

  std::size_t GetTotalAllocated() {
    return m_totalAllocated;
  }

private:
  std::atomic<uint64_t> m_totalAllocated = 0;
};