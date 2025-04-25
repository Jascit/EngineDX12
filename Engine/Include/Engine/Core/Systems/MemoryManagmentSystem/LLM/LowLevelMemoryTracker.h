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
  static void recordAlloc(LLMTags tag, size_t bytes);

  static void recordFree(LLMTags tag, size_t bytes);

  static LLMStats const& GetStats(LLMTags tag);

private:
  static LLMStats& stats(LLMTags tag);
};
