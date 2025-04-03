#include <Include/Engine/Core/Resources/buffers/BaseGPUBuffer.h>

BaseGPUBuffer::~BaseGPUBuffer(){
  shutdown();
}

BaseGPUBuffer::BaseGPUBuffer() : m_CPUBuffer(), m_GPUbuffer(nullptr), BaseUnknown("GPUBuffer") {}

void BaseGPUBuffer::shutdown(){
  if (m_GPUbuffer)
  {
    m_GPUbuffer.Reset();
  }
  m_CPUBuffer.reset();
}

bool BaseGPUBuffer::initialize(size_t size, ID3D12Device10* device){
  if (!m_GPUbuffer)
  {
    D3D12_HEAP_PROPERTIES hp;
    hp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    hp.CreationNodeMask = 0;
    hp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    hp.Type = D3D12_HEAP_TYPE_DEFAULT;
    hp.VisibleNodeMask = 0;
    D3D12_RESOURCE_DESC rd;
    rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    rd.Format = DXGI_FORMAT_UNKNOWN;
    rd.MipLevels = 0;
    rd.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    rd.DepthOrArraySize = 1;
    rd.Flags = D3D12_RESOURCE_FLAG_NONE;
    rd.Height = 1;
    rd.Width = size;
    rd.SampleDesc.Count = 1;
    rd.SampleDesc.Quality = 0;
    HRESULT hr = device->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(m_GPUbuffer.GetAddressOf()));
    if (FAILED(hr))
    {
      ErrorHandler::Get().CatchHRESULT(hr, "failed to create buffer in " + GetName());
      return false;
    }
    m_CPUBuffer.initialize(m_size);
  }
  else
  {
    Logger::Get().logError("GPUBuffer is initialized, can not be initialized one more time!");
    return false;
  }
  return true;
}
