#pragma once
#include <Engine/Core/Systems/ThreadingSystem/ThreadPool/ThreadPool.h>
#include <Engine/Utils/WinInclude.h>
//TODO: Barriers/sync point
class ThreadingSystem {
public:
  template<typename Func>
  static unsigned long createThread(Func&& func);
  template<typename Func, typename... Args>
  static unsigned long createThread(Func&& func, Args&&... arg);
  thread* GetThread(unsigned long ID);

private:
  ThreadPool _pool;
  static std::unordered_map<unsigned long, thread> _aliveThreads;
};

// hi
