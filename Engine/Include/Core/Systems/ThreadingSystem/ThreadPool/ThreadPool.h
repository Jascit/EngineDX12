#pragma once
#include <Core/Systems/ThreadingSystem/ThreadPool/TaskQueue.h>
#include <condition_variable>
#include <thread>
#include <mutex>


#define THREADCOUNT_FOR_THREAD_POOL 4

class ThreadPool {
public:
  ThreadPool();
  void addTask(ITask* task);
  void shutdown();

private:
  void threadCycle();

private:
  std::vector<std::thread> _workerThreads;
  std::mutex _mtx;
  std::condition_variable _cv;
  TaskQueue _queue;
  bool _shouldStop;

};
