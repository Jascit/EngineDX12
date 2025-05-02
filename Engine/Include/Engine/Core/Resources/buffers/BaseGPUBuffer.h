#pragma once
#include <Engine/Utils/WinInclude.h>
#include <Engine/Core/Resources/Container.h>
#include <Engine/Core/Interfaces/BaseObject.h>
//TODO: 3 refactor
class BaseGPUBuffer : BaseObject {
protected:
  ComPtr<ID3D12Resource2> m_GPUbuffer;
  size_t m_size;
public:
  virtual ~BaseGPUBuffer() override;
  BaseGPUBuffer();
 
  bool initialize(size_t size, ID3D12Device10* device);
  void shutdown();

  inline ID3D12Resource* Get() { return m_GPUbuffer.Get(); }
  inline size_t GetSize() { return m_size; }
  inline D3D12_RESOURCE_DESC GetDescriptor() { return m_GPUbuffer->GetDesc(); };
  
};
