#include <Diagnostic/LoggerTest.h>

int main() {
  Logger::Get().clearLogs();
  loggerMultithreadedTest();
  for (size_t i = 0; i < static_cast<int>(LLMTags::Count); i++)
  {
    switch (LLMTags(i))
    {
    case LLMTags::Assets:
      Logger::Get().logInfo("Current tag: Assets");
      break;
    case LLMTags::Physics:
      Logger::Get().logInfo("Current tag: Physics");
      break;
    case LLMTags::Rendering:
      Logger::Get().logInfo("Current tag: Rendering");
      break;
    case LLMTags::Audio:
      Logger::Get().logInfo("Current tag: Audio");
      break;
    case LLMTags::STD:
      Logger::Get().logInfo("Current tag: STD");
      break;
    case LLMTags::Unknown:
      Logger::Get().logInfo("Current tag: Unknown");
      break;
    case LLMTags::Count:
      break;
    default:
      break;
    }
    auto& stats = LLMTracker::GetStats(LLMTags(i));
    Logger::Get().logInfo("Memory allocated: " + std::to_string(stats.allocated) + " memory freed: " + std::to_string(stats.freed));
  }
  Logger::Get().waitForEnd();
  delete GMalloc;
  return 0;
}