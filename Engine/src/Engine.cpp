#include <Include/Engine/Graphics/DebugLayer/DXDebugLayer.h>
#include <Include/Engine/Utils/WinInclude.h>
#include <Include/Engine/Core/DX12/DXContext.h>
#include <Include/Engine/Core/Window/Window.h>
#include <Tests/CoreTests/Memory/GMallocTest.h>
#include <array>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  
  Logger::Get().clearLogs();
  if (DXDebugLayer::Get().initialize())
  {
    Logger::Get().logInfo("DXDebugLayer is initialized");
    if (DXContext::Get().initialize() && Window::Get().initialize())
    {
      Logger::Get().logInfo("DXContex and Window are initialized");
      test();
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