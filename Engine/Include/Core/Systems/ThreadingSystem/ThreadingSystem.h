#pragma once
#include <Core/Systems/ThreadingSystem/ThreadPool/ThreadPool.h>
#include <Utils/WinInclude.h>

//TODO: Barriers/sync point
class ThreadingSystem {
public:
  template<typename Func>
  static unsigned long createThread(Func&& func);
  template<typename Func, typename... Args>
  static unsigned long createThread(Func&& func, Args&&... arg);
  std::thread* GetThread(unsigned long ID);

private:
  ThreadPool _pool;
  static std::unordered_map<unsigned long, std::thread> _aliveThreads;
};

// hi
