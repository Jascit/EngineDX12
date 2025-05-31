#pragma once
#include <Utils/WinInclude.hpp>
#include <Core/DX12/CommandQueue.hpp>
#include <Core/Threading/thread.hpp>
#include <Core/Threading/ConditionalVariables/ConditionalVariableCS.hpp>

class UploadManager {
public:
  UploadManager(CommandQueue* m_uploadQueue);
  void staticUpload(ID3D12Resource2* GPUBuffer);
  void dynamicMapping(ID3D12Resource2* GPUBuffer);
  void waitForUploads();
private:
  void threadFunction();
private:
  CommandQueue* m_uploadQueue;
  thread m_asyncThread;
};
