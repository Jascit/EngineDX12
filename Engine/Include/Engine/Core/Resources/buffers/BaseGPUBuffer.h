#pragma once
#include <Include/Engine/Core/Interfaces/BaseUnknown.h>
#include <Include/Engine/Utils/WinInclude.h>
#include <Include/Engine/Utils/Container.h>

class BaseGPUBuffer : public BaseUnknown {
protected:
  static std::atomic<uint32_t> g_GPUBufferID;
  ComPtr<ID3D12Resource2> m_GPUbuffer;
  size_t m_size;
  RawContainer m_buffer;
public:
  virtual ~BaseGPUBuffer() override;
  BaseGPUBuffer();

  bool initialize(size_t size, ID3D12Device10* device);
  void shutdown() override;

  inline ID3D12Resource* Get() { return m_GPUbuffer.Get(); }
  inline size_t GetSize() { return m_size; }
  inline D3D12_RESOURCE_DESC GetDescriptor() { return m_GPUbuffer->GetDesc(); };

};