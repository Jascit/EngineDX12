#include <Include/Engine/Graphics/DebugLayer/DXDebugLayer.h>
#include <Include/Engine/Utils/WinInclude.h>
#include <Include/Engine/Core/DX12/DXContext.h>
namespace fs = std::filesystem;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  
  Logger::Get().clearLogs();
  if (DXDebugLayer::Get().initialize())
  {
    Logger::Get().logInfo("DXDebugLayer is initialized");
    if (DXContext::Get().initialize())
    {
      Logger::Get().logInfo("DXContex and Window are initialized");

    }
  }
  
  return 0;
}