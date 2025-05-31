#include <Core/DX12/DXContext.h>

DXContext::~DXContext(){
  shutdown();
}
void DXContext::flush(size_t count)
{
  for (size_t i = 0; i < count; i++)
    m_graphicsQueue.waitForGPU();
}

bool DXContext::initialize() {
  HRESULT hr = CreateDXGIFactory2(0, IID_PPV_ARGS(m_factory.GetAddressOf()));
  if (FAILED(hr))
  {
    ErrorHandler::Get().CatchHRESULT(hr, "Failed to create DXGI factory");
    return false;
  }
  hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(m_device.GetAddressOf()));
  if (FAILED(hr))
  {
    ErrorHandler::Get().CatchHRESULT(hr, "Failed to create DXGI factory");
    return false;
  }
  if (!m_graphicsQueue.initialize(m_device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    Logger::Get().logError("Failed to create graphics queue");
    return false;
  }
  if(!m_copyQueue.initialize(m_device.Get(), D3D12_COMMAND_LIST_TYPE_COPY)) {
    ErrorHandler::Get().CatchHRESULT(hr, "Failed to create copy queue");
    return false;
  }
  if (!m_computeQueue.initialize(m_device.Get(), D3D12_COMMAND_LIST_TYPE_COMPUTE)) {
    ErrorHandler::Get().CatchHRESULT(hr, "Failed to create compute queue");
    return false;
  }
  return true;
}

void DXContext::shutdown() {
  if (m_device)
  {
    m_device.Reset();
  }
  if (m_factory)
  {
    m_factory.Reset();
  }
  m_computeQueue.shutdown();
  m_graphicsQueue.shutdown();
  m_copyQueue.shutdown();
}

