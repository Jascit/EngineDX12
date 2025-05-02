#pragma once
#include <Engine/Core/Interfaces/Multithreading/ITask.h>
#include <mutex>
#include <list>
//thread safe
class TaskQueue {
public:
  void push(ITask* task);
  ITask* pop();
  bool empty();
  size_t size();

private:
  std::list<ITask*> m_queue;
  std::mutex _mtx;
};
