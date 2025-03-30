#pragma once
#include <Include/Engine/Utils/WinInclude.h>

class DXDebugLayer
{
public:
  bool initialize();
  void shutdown();
private:
#ifdef _DEBUG
  ComPtr<IDXGIDebug1> m_dxgiInterface;
  ComPtr<ID3D12Debug6> m_d3d12Interface;
#endif // _DEBUG

public:
  DXDebugLayer(const DXDebugLayer&) = delete;
  DXDebugLayer& operator=(const DXDebugLayer&) = delete;
  static inline DXDebugLayer& Get() {
    static DXDebugLayer instance;
    return instance;
  }
private:
  DXDebugLayer() = default;
};

