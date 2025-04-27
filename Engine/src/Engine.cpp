#include <Include/Engine/Graphics/DebugLayer/DXDebugLayer.h>
#include <Tests/CoreTests/Systems/ECS/ECSStressTest.h>
#include <Include/Engine/Core/DX12/DXContext.h>
#include <Include/Engine/Core/Window/Window.h>
#include <Include/Engine/Utils/WinInclude.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

  //Logger::Get().clearLogs();
  if (DXDebugLayer::Get().initialize())
  {
    Logger::Get().logInfo("DXDebugLayer is initialized");
    if (DXContext::Get().initialize() && Window::Get().initialize())
    {
      Logger::Get().logInfo("DXContex and Window are initialized");
      while (!Window::Get().shouldClose())
      {
        Window::Get().update();
        if (Window::Get().shouldResize())
        {
          DXContext::Get().flush(Window::GetFrameCount());
          Window::Get().resize();
        }
        auto* cmdList = DXContext::Get().GetGraphicsQueue()->initCommandList();
        Window::Get().startFrame(cmdList);
        Window::Get().endFrame(cmdList);
        DXContext::Get().GetGraphicsQueue()->excecuteCommandList();
        Window::Get().preset();
      }
    }
    DXContext::Get().flush(Window::GetFrameCount());
    Window::Get().shutdown();
    DXContext::Get().shutdown();
    delete GMalloc;
  }
  DXDebugLayer::Get().shutdown();
  //reallocate this shit
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

  return 0;
}