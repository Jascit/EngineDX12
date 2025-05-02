#pragma once
#include <Engine/Utils/WinInclude.h>
#include <Engine/Core/DX12/CommandQueue.h>
#include <Engine/Core/Threading/thread.h>
#include <Engine/Core/Threading/ConditionalVariables/ConditionalVariableCS.h>

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
