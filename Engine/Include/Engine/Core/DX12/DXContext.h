#pragma once
#include "CommandQueue.h"

class DXContext {
public:
  ~DXContext();

  CommandQueue* getGraphicsQueue() { return &m_graphicsQueue; }
  CommandQueue* getComputeQueue() { return &m_computeQueue; }
  CommandQueue* getCopyQueue() { return &m_copyQueue; }
  ID3D12Device* GetDevice() { return m_device.Get(); }
  IDXGIFactory7* GetFactory() { return m_factory.Get(); }

  bool initialize();
  void shutdown();

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

