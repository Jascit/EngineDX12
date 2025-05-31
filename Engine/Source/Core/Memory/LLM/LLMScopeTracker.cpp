#include <Core/Memory/LLM/LLMScopeTracker.h>
LLMTrackerScope::LLMTrackerScope(LLMTags tag, const char* file, size_t line, const char* func)
  : _tag(tag), _file(file), _line(line), _func(func)
{
  // Optional: Logging or Debug
  GCurrentLLMTag = tag;
}

LLMTrackerScope::~LLMTrackerScope() {
  GCurrentLLMTag = LLMTags::Unknown;
}
