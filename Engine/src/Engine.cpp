#include <Include/Engine/Graphics/DebugLayer/DXDebugLayer.h>
#include <Include/Engine/Utils/WinInclude.h>

namespace fs = std::filesystem;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  
  Logger::Get();
  if (DXDebugLayer::Get().initialize())
  {
    Logger::Get().logInfo("DXDebugLayer is initialized");
  }
  
  return 0;
}