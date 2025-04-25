#pragma once
#include <Include/Engine/Core/Threading/ConditionalVariables/ConditionalVariableCS.h>
#include <Include/Engine/Core/Systems/ThreadingSystem/ThreadPool/TaskQueue.h>
#include <Include/Engine/Core/Threading/CriticalSections/CriticalSection.h>
#include <Include/Engine/Core/Threading/Lockers/LockGuard.h>
#include <Include/Engine/Core/Threading/thread.h>
#define THREADCOUNT_FOR_THREAD_POOL 4

class ThreadPool {
public:
  ThreadPool();
  void addTask(ITask* task);
  void shutdown();

private:
  void threadCycle();

private:
  std::vector<thread> _workerThreads;
  CriticalSection _cs;
  ConditionalVariableCS _cv;
  TaskQueue _queue;
  bool _shouldStop;

};