#pragma once
#include <atomic>

enum class LLMTags : uint8_t {
  Assets = 0,
  Physics,
  Rendering,
  Audio,
  Unknown,
  STD,
  Count
};
struct LLMStats {
  std::atomic<size_t> allocated{ 0 };
  std::atomic<size_t> freed{ 0 };
};
extern thread_local LLMTags GCurrentLLMTag;