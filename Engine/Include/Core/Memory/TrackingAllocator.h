#pragma once
#include <Core/Memory/LLM/LLMTracker.h>
#include <Core/Interfaces/IMalloc.h>
#include <vector>
#include <cstddef>
#include <new>
#include <type_traits>
#include <cassert>
#include <utility>

// Maximum alignment to reserve space for metadata
constexpr size_t MaxAllocationAlignment = 64;
// Default alignment if none is specified
constexpr size_t DefaultAllocationAlignment = 8;


struct MetadataPtr {
  void* _basePtr;       // Pointer to the full allocated block (metadata + data)
  void* _userPtr;       // Pointer returned to the user (after metadata)
  uint32_t _totalSize;  // Total size of the allocation (metadata + user data)
  uint32_t _alignment;  // Alignment used for the allocation
  uint32_t _freeBytes;  // Free bytes
  uint32_t _bytesInUse; // Bytes currently in use

  MetadataPtr(void* basePtr, void* userPtr, uint32_t totalSize, uint32_t alignment, uint32_t freeBytes, uint32_t bytesInUse)
    : _basePtr(basePtr), _userPtr(userPtr), _totalSize(totalSize), _alignment(alignment),
    _freeBytes(freeBytes), _bytesInUse(bytesInUse) {
  };
};

constexpr size_t MetaDataPadding = sizeof(MetadataPtr);

struct AllocInfo {
  MetadataPtr* _metadata; // Points to allocation metadata
  void* _userData;        // User-accessible data pointer

  AllocInfo(MetadataPtr* metadata, void* userData)
    : _metadata(metadata), _userData(userData) {
  }
};

class TrackingAllocator : public Malloc {
public:
  void* allocate(uint32_t bytes, uint32_t alignment = DefaultAllocationAlignment) override;
  AllocInfo alignedAllocate(uint32_t bytes, uint32_t alignment = DefaultAllocationAlignment);
  void deallocate(void* ptr) noexcept override;
  inline void* userPtrFromBase(void* ptr);
  inline MetadataPtr* metadataFromPtr(void* ptr);

  template<typename U, typename... Args>
  void construct(U* p, Args&&... args) {
    ::new(static_cast<void*>(p)) U(std::forward<Args>(args)...);
  }
};

extern TrackingAllocator* GMalloc;
