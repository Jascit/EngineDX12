#include <Core/DX12/CommandQueue.hpp>

std::atomic<uint32_t> CommandQueue::g_commandQueueID = 0;

bool CommandQueue::initialize(ID3D12Device10* device, D3D12_COMMAND_LIST_TYPE type) {
  D3D12_COMMAND_QUEUE_DESC cqd;
  cqd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  cqd.NodeMask = 0;
  cqd.Priority = (type == D3D12_COMMAND_LIST_TYPE_DIRECT) ? D3D12_COMMAND_QUEUE_PRIORITY_HIGH : D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
  cqd.Type = type;
  HRESULT hr = device->CreateCommandQueue(&cqd, IID_PPV_ARGS(m_queue.GetAddressOf()));
  if (FAILED(hr))
  {
    ErrorHandler::Get().CatchHRESULT(hr, "failed to create command queue");
    return false;
  }
  hr = device->CreateCommandAllocator(type, IID_PPV_ARGS(m_allocator.GetAddressOf()));
  if (FAILED(hr))
  {
    ErrorHandler::Get().CatchHRESULT(hr, "failed to create command allocator");
    return false;
  }
  hr = device->CreateCommandList1(0, type, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(m_commandList.GetAddressOf()));
  if (FAILED(hr))
  {
    ErrorHandler::Get().CatchHRESULT(hr, "failed to create command list");
    return false;
  }
  hr = device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf()));
  if (FAILED(hr))
  {
    ErrorHandler::Get().CatchHRESULT(hr, "failed to create command list");
    return false;
  }
  m_fenceEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
  return true;
}

bool CommandQueue::initialize(ID3D12Device10* device, D3D12_COMMAND_LIST_TYPE type, ID3D12PipelineState* pipelineState) {
  D3D12_COMMAND_QUEUE_DESC cqd;
  cqd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  cqd.NodeMask = 0;
  cqd.Priority = (type == D3D12_COMMAND_LIST_TYPE_DIRECT) ? D3D12_COMMAND_QUEUE_PRIORITY_HIGH : D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
  cqd.Type = type;
  HRESULT hr = device->CreateCommandQueue(&cqd, IID_PPV_ARGS(m_queue.GetAddressOf()));
  if (FAILED(hr))
  {
    ErrorHandler::Get().CatchHRESULT(hr, "failed to create command queue");
    return false;
  }
  hr = device->CreateCommandAllocator(type, IID_PPV_ARGS(m_allocator.GetAddressOf()));
  if (FAILED(hr))
  {
    ErrorHandler::Get().CatchHRESULT(hr, "failed to create command allocator");
    return false;
  }
  hr = device->CreateCommandList1(0, type, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(m_commandList.GetAddressOf()));
  if (FAILED(hr))
  {
    ErrorHandler::Get().CatchHRESULT(hr, "failed to create command list");
    return false;
  }
  hr = device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf()));
  if (FAILED(hr))
  {
    ErrorHandler::Get().CatchHRESULT(hr, "failed to create command list");
    return false;
  }
  m_fenceEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
  return true;
}

CommandQueue::CommandQueue() : m_fenceValue(0), m_fenceEvent(nullptr){}

CommandQueue::~CommandQueue(){
  shutdown();
}

void CommandQueue::shutdown(){
  if (m_queue)
  {
    m_queue.Reset();
  }
  if (m_allocator)
  {
    m_allocator.Reset();
  }
  if (m_commandList)
  {
    m_commandList.Reset();
  }
  if (m_fence)
  {
    m_fence.Reset();
  }
  if (m_fenceEvent) {
    CloseHandle(m_fenceEvent);
    m_fenceEvent = nullptr;
  }
}

void CommandQueue::excecuteCommandList(){
  if (SUCCEEDED(m_commandList->Close())) {
    ID3D12CommandList* lists[] = { m_commandList.Get() };
    m_queue->ExecuteCommandLists(1, lists);
    waitForGPU();
  }
}
ID3D12GraphicsCommandList7* CommandQueue::initCommandList() {
  m_allocator->Reset();
  m_commandList->Reset(m_allocator.Get(), nullptr);
  return m_commandList.Get();
}

void CommandQueue::waitForGPU(){
  m_queue->Signal(m_fence.Get(), ++m_fenceValue);
  if (SUCCEEDED(m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent)))
  {
    if (WaitForSingleObject(m_fenceEvent, 20000) != WAIT_OBJECT_0)
    {
      Logger::Get().logError("Signal didn't come back from GPU, comand queue's name: " + GetName());
      std::exit(-1);
    }
  }
  else {
    Logger::Get().logError("Failed to set event on fence completion! Comand queue's name: " + GetName());
    std::exit(-1);
  }
}

