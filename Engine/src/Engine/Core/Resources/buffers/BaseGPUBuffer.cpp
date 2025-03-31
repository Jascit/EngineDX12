#include <Include/Engine/Core/Resources/buffers/BaseGPUBuffer.h>
std::atomic<uint32_t> BaseGPUBuffer::g_GPUBufferID = 0;

BaseGPUBuffer::~BaseGPUBuffer(){
  shutdown();
}

BaseGPUBuffer::BaseGPUBuffer() : m_buffer(m_size), m_GPUbuffer(nullptr), BaseUnknown("GPUBuffer", g_GPUBufferID) {}

void BaseGPUBuffer::shutdown(){
  if (m_GPUbuffer)
  {
    m_GPUbuffer.Reset();
  }
  if (m_buffer)
  {
    free(m_buffer);
  }
}

bool BaseGPUBuffer::initialize(size_t size, ID3D12Device10* device){
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
  }
  m_buffer = malloc()
}
