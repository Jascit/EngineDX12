#include <Include/Engine/Core/Systems/MemoryManagmentSystem/LLM/LowLevelMemoryTracker.h>

void LowLevelMemoryTracker::recordAlloc(LLMTags tag, size_t bytes) {
  stats(tag).allocated += bytes;
}

void LowLevelMemoryTracker::recordFree(LLMTags tag, size_t bytes) {
  stats(tag).freed += bytes;
}

LLMStats const& LowLevelMemoryTracker::GetStats(LLMTags tag) {
  return stats(tag);
}

LLMStats& LowLevelMemoryTracker::stats(LLMTags tag) {
  static std::array<LLMStats, static_cast<size_t>(LLMTags::Count)> statsArray;
  return statsArray[static_cast<size_t>(tag)];
}