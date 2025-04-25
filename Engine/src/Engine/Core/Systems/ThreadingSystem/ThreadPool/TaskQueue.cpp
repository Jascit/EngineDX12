#include <Include/Engine/Core/Systems/ThreadingSystem/ThreadPool/TaskQueue.h>

void TaskQueue::push(ITask* task) {
  if (task == nullptr) return;
  LockGuard<CriticalSection> lock(_cs);
  m_queue.push_back(task);
}

ITask* TaskQueue::pop() {
  LockGuard<CriticalSection> lock(_cs);
  if (m_queue.empty()) return nullptr;

  ITask* instance = m_queue.front();
  m_queue.pop_front();
  return instance;
}

bool TaskQueue::empty() {
  LockGuard<CriticalSection> lock(_cs);
  return m_queue.empty();
}

size_t TaskQueue::size() {
  LockGuard<CriticalSection> lock(_cs);
  return m_queue.size();
}
