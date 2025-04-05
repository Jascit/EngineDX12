#pragma once
#include <Include/Engine/Utils/WinInclude.h>
#include <Include/Engine/Core/DX12/CommandQueue.h>
#include <Include/Engine/Core/Threading/thread.h>
#include <Include/Engine/Core/Threading/ConditionalVariables/ConditionalVariableCS.h>

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