#include <Engine/Core/Memory/TrackingAllocator.h>

TrackingAllocator* GMalloc = new TrackingAllocator;

void* TrackingAllocator::allocate(uint32_t bytes, uint32_t alignment) {
  try {
    uint32_t totalSize = bytes + MaxAllocationAlignment;
    totalSize = (totalSize + alignment - 1) & ~(alignment - 1);
    void* base = ::operator new(totalSize, std::align_val_t{ alignment });
    MetadataPtr* data = reinterpret_cast<MetadataPtr*>(base);
    new(data) MetadataPtr(base, userPtrFromBase(base), totalSize, alignment,
      static_cast<uint32_t>(totalSize - MaxAllocationAlignment), 0);
    LLMTracker::recordAlloc(GCurrentLLMTag, totalSize);
    return data->_userPtr;
  }
  catch (const std::exception&) {
    return nullptr;
  }
}

AllocInfo TrackingAllocator::alignedAllocate(uint32_t bytes, uint32_t alignment) {
  try {
    uint32_t totalSize = bytes + MaxAllocationAlignment;
    totalSize = (totalSize + alignment - 1) & ~(alignment - 1);
    void* base = ::operator new(totalSize, std::align_val_t{ alignment });
    MetadataPtr* data = reinterpret_cast<MetadataPtr*>(base);
    new(data) MetadataPtr(base, userPtrFromBase(base), totalSize, alignment,
      static_cast<uint32_t>(totalSize - MaxAllocationAlignment), 0);
    LLMTracker::recordAlloc(GCurrentLLMTag, totalSize);
    return AllocInfo(data, userPtrFromBase(base));
  }
  catch (const std::exception&) {
    return AllocInfo(nullptr, nullptr);
  }
}

void TrackingAllocator::deallocate(void* ptr) noexcept {
  if (!ptr)
    return;

  MetadataPtr* metadata = metadataFromPtr(ptr);

  assert(metadata && "Invalid metadata pointer in deallocate.");
  assert(metadata->_basePtr && "Base pointer is missing in metadata.");
  assert(metadata->_totalSize > 0 && "Invalid size in metadata.");
  assert((reinterpret_cast<uintptr_t>(metadata->_basePtr) % metadata->_alignment) == 0 && "Misaligned base pointer.");
  LLMTracker::recordFree(GCurrentLLMTag, metadata->_totalSize);
  ::operator delete(metadata->_basePtr, metadata->_totalSize, std::align_val_t{ metadata->_alignment });
}

inline void* TrackingAllocator::userPtrFromBase(void* ptr) {
  return reinterpret_cast<char*>(ptr) + MaxAllocationAlignment;
}

inline MetadataPtr* TrackingAllocator::metadataFromPtr(void* ptr) {
  return reinterpret_cast<MetadataPtr*>(reinterpret_cast<char*>(ptr) - MaxAllocationAlignment);
}
