#pragma once
#include <Core/DX12/CommandQueue.h>

class DXContext {
public:
  ~DXContext();

  CommandQueue* GetGraphicsQueue() { return &m_graphicsQueue; }
  CommandQueue* GetComputeQueue() { return &m_computeQueue; }
  CommandQueue* GetCopyQueue() { return &m_copyQueue; }
  ID3D12Device* GetDevice() { return m_device.Get(); }
  IDXGIFactory7* GetFactory() { return m_factory.Get(); }

  bool initialize();
  void shutdown();

  void flush(size_t count);

private:

  ComPtr<IDXGIFactory7> m_factory;
  ComPtr<ID3D12Device10> m_device;

  CommandQueue m_graphicsQueue;
  CommandQueue m_computeQueue;
  CommandQueue m_copyQueue;
public:
  DXContext(const DXContext&) = delete;
  DXContext& operator=(const DXContext&) = delete;
  static inline DXContext& Get() {
    static DXContext instance;
    return instance;
  }
private:
  DXContext() = default;
};

