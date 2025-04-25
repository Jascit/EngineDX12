#pragma once
#include <Include/Engine/Core/Systems/MemoryManagmentSystem/LLM/LLMStructs.h>

class LLMTrackerScope {
public:
  LLMTrackerScope(LLMTags tag, const char* file, size_t line, const char* func)
    : _tag(tag), _file(file), _line(line), _func(func)
  {
    // Optional: z.?B. Logging oder Debug-Ausgabe
    GCurrentLLMTag = static_cast<int>(tag);
  }

  ~LLMTrackerScope() {
    // Scope endet – zur?cksetzen oder aufr?umen
    GCurrentLLMTag = static_cast<int>(LLMTags::Unknown);
  }

private:
  LLMTags _tag;
  const char* _file;
  size_t _line;
  const char* _func;
};