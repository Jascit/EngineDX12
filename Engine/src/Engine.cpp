#include <Include/Engine/Graphics/DebugLayer/DXDebugLayer.h>
#include <Include/Engine/Utils/WinInclude.h>
#include <Include/Engine/Core/DX12/DXContext.h>
#include <Include/Engine/Core/Window/Window.h>
#include <Tests/CoreTests/Memory/GMallocTest.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  
  Logger::Get().clearLogs();
  if (DXDebugLayer::Get().initialize())
  {
    Logger::Get().logInfo("DXDebugLayer is initialized");
    if (DXContext::Get().initialize() && Window::Get().initialize())
    {
      Logger::Get().logInfo("DXContex and Window are initialized");
      double gmalloc = 0;
      double standartnew = 0;
      for (size_t i = 0; i < 20; i++)
      {
        standartnew += testStandardNew();
        gmalloc += testGMalloc();
      }
      Logger::Get().logInfo("Standart new average time: " + std::to_string(standartnew/20) + " ms\n");
      Logger::Get().logInfo("GMalloc average time: " + std::to_string(gmalloc / 20) + " ms\n");
      //while (!Window::Get().shouldClose())
      //{
      //  Window::Get().update();
      //  if (Window::Get().shouldResize())
      //  {
      //    DXContext::Get().flush(Window::GetFrameCount());
      //    Window::Get().resize();
      //  }
      //  auto* cmdList = DXContext::Get().GetGraphicsQueue()->initCommandList();
      //  Window::Get().startFrame(cmdList);
      //  Window::Get().endFrame(cmdList);
      //  DXContext::Get().GetGraphicsQueue()->excecuteCommandList();
      //  Window::Get().preset();
      //}
    }
    DXContext::Get().flush(Window::GetFrameCount());
    Window::Get().shutdown();
    DXContext::Get().shutdown();
    delete GMalloc;
  }
  DXDebugLayer::Get().shutdown();
  
  return 0;
}