#pragma once
#include <Core/Memory/LLM/LLMStructs.h>

//TODO: Allocators remake
class LLMTrackerScope {
public:
  LLMTrackerScope(LLMTags tag, const char* file, size_t line, const char* func);
  ~LLMTrackerScope();
private:
  LLMTags _tag;
  const char* _file;
  size_t _line;
  const char* _func;
};
