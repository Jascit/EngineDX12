#include <Engine/Core/Systems/ThreadingSystem/ThreadPool/TaskQueue.h>

void TaskQueue::push(ITask* task) {
  if (task == nullptr) return;
  std::lock_guard<std::mutex> lock(_mtx);
  m_queue.push_back(task);
}

ITask* TaskQueue::pop() {
  std::lock_guard<std::mutex> lock(_mtx);
  if (m_queue.empty()) return nullptr;

  ITask* instance = m_queue.front();
  m_queue.pop_front();
  return instance;
}

bool TaskQueue::empty() {
  std::lock_guard<std::mutex> lock(_mtx);
  return m_queue.empty();
}

size_t TaskQueue::size() {
  std::lock_guard<std::mutex> lock(_mtx);
  return m_queue.size();
}
