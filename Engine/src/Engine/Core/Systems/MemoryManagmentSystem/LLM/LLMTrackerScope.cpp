#include <Include/Engine/Core/Systems/MemoryManagmentSystem/LLM/LLMTrackerScope.h>
LLMTrackerScope::LLMTrackerScope(LLMTags tag, const char* file, size_t line, const char* func)
  : _tag(tag), _file(file), _line(line), _func(func)
{
  // Optional: z.?B. Logging oder Debug-Ausgabe
  GCurrentLLMTag = static_cast<int>(tag);
}

LLMTrackerScope::~LLMTrackerScope() {
  // Scope endet – zur?cksetzen oder aufr?umen
  GCurrentLLMTag = static_cast<int>(LLMTags::Unknown);
}
