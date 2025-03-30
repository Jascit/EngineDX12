#pragma once
#include "CommandQueue.h"

class DXContext {
public:
  DXContext(HWND hwnd);
  ~DXContext();

  CommandQueue* getGraphicsQueue() { return &graphicsQueue; }
  CommandQueue* getComputeQueue() { return &computeQueue; }
  CommandQueue* getCopyQueue() { return &copyQueue; }

private:
  void initDevice();
  void createSwapChain();

  ComPtr<ID3D12Device> device;
  ComPtr<IDXGISwapChain3> swapChain;

  CommandQueue graphicsQueue;
  CommandQueue computeQueue;
  CommandQueue copyQueue;
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

