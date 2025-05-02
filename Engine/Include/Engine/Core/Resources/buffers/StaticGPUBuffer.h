#pragma once
#include "BaseGPUBuffer.h"
#include <Engine/Core/DX12/DXContext.h>

template<typename T>
class StaticGPUBuffer : public BaseGPUBuffer {
public:
  StaticGPUBuffer() : m_sizeOfELement(sizeof(T)), m_currentOffset(0) {}
  StaticGPUBuffer(T* data, size_t size) : m_sizeOfELement(sizeof(T)), m_currentOffset(0) {
    size_t alignmentSize = (size + D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT - 1) & D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    initialize(alignmentSize, DXContext::Get().GetDevice());
    m_CPUBuffer.copyData(data, size);
  }

  void update(const T* data, size_t numElements, size_t offset = 0) {
    memcpy((char*)m_CPUBuffer.GetData() + offset, data, numElements * m_sizeOfELement);
  }

  bool resize(size_t newSize) {
  
  }
  
  void addData(const T* data, size_t numElements) {
    if (m_currentOffset + numElements * m_sizeOfELement > m_size)
    {
      Logger::Get().logError("Not enough space in buffers");
      return;
    }
    m_CPUBuffer.addData(data, numElements * m_sizeOfELement, m_currentOffset);
    m_currentOffset += numElements * m_sizeOfELement;
    
  }

  const T* GetData(size_t offset = 0) {
    return reinterpret_cast<T*>((char*)m_CPUBuffer.GetData() + offset);
  }

private:
  size_t m_sizeOfELement;
  size_t m_currentOffset;
};
