#include <Core/Memory/LLM/LLMTracker.h>

void LLMTracker::recordAlloc(LLMTags tag, size_t bytes) {
  stats(tag).allocated += bytes;
}

void LLMTracker::recordFree(LLMTags tag, size_t bytes) {
  stats(tag).freed += bytes;
}

LLMStats const& LLMTracker::GetStats(LLMTags tag) {
  return stats(tag);
}

LLMStats& LLMTracker::stats(LLMTags tag) {
  static std::array<LLMStats, static_cast<size_t>(LLMTags::Count)> statsArray;
  return statsArray[static_cast<size_t>(tag)];
}
