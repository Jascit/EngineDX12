#pragma once
#include <Utils/WinInclude.h>
#include <Core/DX12/CommandQueue.h>
#include <Core/Threading/thread.h>
#include <Core/Threading/ConditionalVariables/ConditionalVariableCS.h>

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
