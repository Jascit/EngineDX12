#pragma once
#include <Core/Interfaces/BaseObject.hpp>
#include <Utils/WinInclude.hpp>


class CommandQueue : public BaseObject {
protected:
  static std::atomic<uint32_t> g_commandQueueID;
public:
  bool initialize(ID3D12Device10* device, D3D12_COMMAND_LIST_TYPE type);
  bool initialize(ID3D12Device10* device, D3D12_COMMAND_LIST_TYPE type, ID3D12PipelineState* pipelineState);
  CommandQueue();
  ~CommandQueue() override;

  void waitForGPU();
  void shutdown();

  ID3D12GraphicsCommandList7* initCommandList();
  void excecuteCommandList();

  ID3D12CommandQueue* getQueue() const { return m_queue.Get(); }
  ID3D12CommandAllocator* getAllocator() const { return m_allocator.Get(); }
  ID3D12GraphicsCommandList7* getCommandList() const { return m_commandList.Get(); }

private:
  ComPtr<ID3D12CommandQueue> m_queue;
  ComPtr<ID3D12CommandAllocator> m_allocator;
  ComPtr<ID3D12GraphicsCommandList7> m_commandList;
  ComPtr<ID3D12Fence> m_fence;
  HANDLE m_fenceEvent;
  UINT64 m_fenceValue;
};
