#pragma once
#include <Include/Engine/Core/Systems/MemoryManagmentSystem/LLM/LLMTrackerScope.h>
#include <atomic>
#include <array>
#define LLM_SCOPE_BYTAG(Tag) \
    LowLevelMemoryTracker::recordAlloc(Tag, 0); /* Reset? optional */ \
    LLMTrackerScope tracker##__LINE__(Tag, __FILE__, __LINE__, __func__);
//TODO: create debug functions and macros

class LowLevelMemoryTracker {
public:
  static void recordAlloc(LLMTags tag, size_t bytes) {
    getStats(tag).allocated += bytes;
  }

  static void recordFree(LLMTags tag, size_t bytes) {
    getStats(tag).freed += bytes;
  }

  static LLMStats const& stats(LLMTags tag) {
    return getStats(tag);
  }

private:
  static LLMStats& getStats(LLMTags tag) {
    static std::array<LLMStats, static_cast<size_t>(LLMTags::Count)> statsArray;
    return statsArray[static_cast<size_t>(tag)];
  }
};
