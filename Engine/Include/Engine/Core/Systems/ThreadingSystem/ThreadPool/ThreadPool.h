#pragma once
#include <Engine/Core/Threading/ConditionalVariables/ConditionalVariableCS.h>
#include <Engine/Core/Systems/ThreadingSystem/ThreadPool/TaskQueue.h>
#include <Engine/Core/Threading/CriticalSections/CriticalSection.h>
#include <Engine/Core/Threading/Lockers/LockGuard.h>
#include <Engine/Core/Threading/thread.h>
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
