#pragma once
#include <Include/Engine/Core/Interfaces/Multithreading/ITask.h>
#include <Include/Engine/Core/Threading/CriticalSections/CriticalSection.h>
#include <Include/Engine/Core/Threading/Lockers/LockGuard.h>
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
  CriticalSection _cs;
};