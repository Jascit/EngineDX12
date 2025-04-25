#pragma once
#include <Include/Engine/Core/Systems/MemoryManagmentSystem/LLM/LowLevelMemoryTracker.h>
#include <Include/Engine/Core/Diagnostics/ErrorHandler.h>
#include <Include/Engine/Core/Interfaces/IAllocator.h>
#include <unordered_map>
#include <string>
#include <atomic>

//Speaks with GCS through events
class MemoryManagmentSystem {
public:
  struct Stats {
    std::atomic<size_t> totalAllocated = 0;
    std::atomic<size_t> totalFreed = 0;
    std::atomic<size_t> currentUsage = 0;
    std::atomic<size_t> peakUsage = 0;
  };

  uint32_t registerAllocator(IAllocator* allocator);
  void unregisterAllocator(uint32_t ID);

  Stats const& GetStats(uint32_t ID) const;
  Stats        GetTotalStats()      const;

  void onAlloc(uint32_t ID, size_t bytes);
  void onFree(uint32_t ID, size_t bytes);


  void setLimit(uint32_t ID, size_t maxBytes);
  size_t getLimit(uint32_t ID) const;

  void checkForLeaks() const;
  void logStats()      const;

  IAllocator* findAlloc(uint32_t ID);

  template<typename Tag>
  IAllocator* GetAllocator() {
    auto& [alloc, stats] = GetPair(AllocatorID<Tag>);
    stats.objectsInUse.fetch_add(1, std::memory_order_relaxed);
    return alloc;
  }

  template<typename Tag>
  Stats const& GetStats() const {
    return GetStats(AllocatorName<Tag>);
  }

  template<typename Tag>
  IAllocator* GetAllocatorCached() {
    static IAllocator* cached = MemoryManagmentSystem::instance().GetAllocator<Tag>();
    return cached;
  }
  template<typename Tag>
  Stats const& GetStatsCached() const {
    static Stats const& instance = GetStats(AllocatorName<Tag>);
    return GetStats(AllocatorName<Tag>);
  }

private:
  auto GetPair(uint32_t id) -> std::pair<IAllocator*, Stats>& {
    return _allocatorsData.at(id);
  }

private:
  std::unordered_map<uint32_t, std::pair<IAllocator*, Stats>> _allocatorsData;
  std::atomic<uint32_t> _currentAllocID;

public:
  static MemoryManagmentSystem& instance() {
    static MemoryManagmentSystem instance;
    return instance;
  }
};

extern MemoryManagmentSystem& MemoryService;