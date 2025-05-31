#include <Graphics/DebugLayer/DXDebugLayer.hpp>
#include <Core/Memory/TrackingAllocator.hpp>
#include <Core/DX12/DXContext.hpp>
#include <Core/Window/Window.hpp>
#include <Utils/WinInclude.hpp>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

  Logger::Get().clearLogs();
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
        Sleep(6);
      }
    }
    DXContext::Get().flush(Window::GetFrameCount());
    Window::Get().shutdown();
    DXContext::Get().shutdown();
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
  Logger::Get().waitForEnd();
  delete GMalloc;
  return 0;
}
